#include "IMSClient.h"
#include <QMessageBox>
#include <QString>
#include "CAppClient.h"
#include "CCinData.h"
#include "CIniConfig.h"
#include "json/value.h"
#include "get_mac_address.h"
#include "CEDString.h"



#define _QS(str) QString::fromLocal8Bit(str)


IMSClient::IMSClient(QWidget* parent)
    : QMainWindow(parent)
    , m_app(nullptr)
    , m_entryPage(nullptr)
    , m_setUserNamePtr(nullptr)
    , m_sendData(nullptr)
    , m_myrev(nullptr)
    , m_timer(nullptr)
    , m_currentSendMsgAccount("")
{
    ui.setupUi(this);
    this->setHidden(true);
    this->setVisible(false);

    m_vecClientInfos.clear();
    m_vecClientMsgInfos.clear();

    //setWindowFlags(Qt::Dialog   /* |Qt::FramelessWindowHint*/);

    m_timer = new QTimer(this);

    m_app = new CAppClient();
    m_sendData = new CCinData(m_app);
    m_myrev = new MyReceive(m_app->m_mysocket);
    m_entryPage = new EntryPage(this);
    m_setUserNamePtr = new SetUserName(this);
    InitSignal();
    initWidget();

    m_app->run();
}

IMSClient::~IMSClient()
{
    if (m_entryPage) {
        m_entryPage->close();
        delete m_entryPage;
        m_entryPage = nullptr;
    }
    if (m_setUserNamePtr) {
        m_setUserNamePtr->close();
        delete m_setUserNamePtr;
        m_setUserNamePtr = nullptr;
    }
    if (m_myrev) {
        delete m_myrev;
        m_myrev = nullptr;
    }
    if (m_sendData) {
        delete m_sendData;
        m_sendData = nullptr;
    }
    if (m_app) {
        m_app->exitinstance();
        delete m_app;
        m_app = nullptr;
    }
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
}

void IMSClient::InitSignal()
{
    connect(m_sendData, &CCinData::SendDataEvent, m_app, &CAppClient::SendData);
    connect(m_myrev, &MyReceive::verifyAccountWithPwdResult, this, &IMSClient::onResult_VerifyUserInfo);
    connect(m_myrev, &MyReceive::ReceiveBroadcastMsg, this, &IMSClient::onResult_BroadcastMsg);
    connect(m_myrev, &MyReceive::verifyAccountIdResult, this, &IMSClient::onResult_RegisterAccountId);

    connect(m_myrev, &MyReceive::ReceiveTranspondMsg, this, &IMSClient::onResult_TranspondMsg);

    connect(m_entryPage, &EntryPage::LoginResult, this, &IMSClient::onEntryPage_LoginResult);
    connect(m_entryPage, &EntryPage::VerifyUserInfo, this, &IMSClient::onDeal_VerifyUserInfo);

    connect(ui.pushButton_message, &QPushButton::clicked, this, &IMSClient::onPushButton_MessageClicked);
    connect(ui.pushButton_persons, &QPushButton::clicked, this, &IMSClient::onPushButton_PersonsClicked);
    connect(m_app, &CAppClient::IsConnectServer, this, &IMSClient::onReceive_ConnectServer);
    connect(this, &IMSClient::UpdateUserInfo, this, &IMSClient::updateTableView_userData);
    connect(this, &IMSClient::UpdateUserMsg, this, &IMSClient::updateTableView_userMsg);
    connect(ui.pushButton_sendMsg, &QPushButton::clicked, this, &IMSClient::onPushButton_sendMsgClick);

    connect(m_timer, &QTimer::timeout, this, &IMSClient::onSend_clientOnLine);

}

void IMSClient::initWidget()
{
    ui.tabWidget_main->tabBar()->hide();
    ui.tabWidget_main->setCurrentIndex(1);

    QStringList headerMsgLists;
    headerMsgLists << _QS("消息") ;
    ui.tableWidget_msgData->setColumnCount(headerMsgLists.size());
    ui.tableWidget_msgData->setHorizontalHeaderLabels(headerMsgLists);
    ui.tableWidget_msgData->setEditTriggers(QAbstractItemView::NoEditTriggers); //表格内容不可编辑
    ui.tableWidget_msgData->verticalHeader()->setHidden(true);  //隐藏行号列
    ui.tableWidget_msgData->setSelectionMode(QAbstractItemView::NoSelection);
    ui.tableWidget_msgData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidget_msgData->setWordWrap(true);


    QStringList headerLists;
    headerLists << _QS("在线好友") << _QS("状态") << _QS("账号");
    ui.tableWidget_userData->setColumnCount(headerLists.size());
    ui.tableWidget_userData->setHorizontalHeaderLabels(headerLists);
    ui.tableWidget_userData->setEditTriggers(QAbstractItemView::NoEditTriggers); //表格内容不可编辑
    ui.tableWidget_userData->verticalHeader()->setHidden(true);  //隐藏行号列
    ui.tableWidget_userData->hideColumn(1);
    ui.tableWidget_userData->hideColumn(2);
    ui.tableWidget_userData->setSelectionBehavior(QAbstractItemView::SelectRows);  //行选择
    //ui.tableWidget_userData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //设置表头自适应宽度
    ui.tableWidget_userData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui.tableWidget_userData->setColumnWidth(0, 100);
    ui.tableWidget_userData->setWordWrap(true);
    connect(ui.tableWidget_userData, &QTableWidget::itemDoubleClicked, this, &IMSClient::onUserData_itemDoubleClicked);

}

bool IMSClient::showLoginWidget()
{
    if (m_entryPage) {
        m_entryPage->show();
        return true;
    } else {
        qApp->exit(EXIT_SUCCESS);
        return false;
        //QCoreApplication::quit();
    }
}

void IMSClient::closeEvent(QCloseEvent* event)
{
    int ret = QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("您确认退出应用程序吗？"), QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"));
    if (0 == ret) {
        if (m_timer) {
            m_timer->stop();
        }
        event->accept();
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
}

void IMSClient::onEntryPage_LoginResult(int ret, QString str)
{
    if (ret == 0) {
        std::string currentName = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME);
        if (currentName.empty()) {
            if (m_setUserNamePtr) {
                m_setUserNamePtr->show();
            }
        } else {
            QString name = QString::fromStdString(currentName.c_str());
            this->setWindowTitle(name);
        }
        this->show();

        m_currentAccount = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT).c_str();
        m_currentAccountName = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME).c_str();
        //广播登录消息
        Json::Value root;
        root[SEND_TYPE] = DataType::eBroadcast;
        root[SEND_ACCOUNT] = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT);
        root[SEND_ACCOUNT_NAME] = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME);
        std::string* strData = new std::string(root.toStyledString());
        m_sendData->SendMsgData(strData);

        //QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("登录成功..."), QString::fromLocal8Bit("确定"));
        if (m_timer) {
            m_timer->start(ON_LINE_TIME);
        }

    } else {
        this->setVisible(false);
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("登录失败，请核查账号信息！"), QString::fromLocal8Bit("确定"));
        m_entryPage->show();
    }
}


void IMSClient::onPushButton_MessageClicked(bool clicked)
{
    ui.tabWidget_main->setCurrentIndex(0);
}

void IMSClient::onPushButton_PersonsClicked(bool clicked)
{
    ui.tabWidget_main->setCurrentIndex(1);
}

void IMSClient::onDeal_VerifyUserInfo(const int& type, const QString& strName, const QString& strPwd)
{
    std::string strMac = "";
    GetMacAddress(strMac);
    if (type == 0) {
        // 校验登录信息是否正确
        Json::Value root;
        root[SEND_TYPE] = DataType::eVerify;
        root[SEND_ACCOUNT] = strName.toStdString();
        root[SEND_PASSWORD] = std::string(CSEncrypt(strPwd.toStdString().c_str()));
        root[SEND_MAC] = strMac;
        std::string* strData = new std::string(root.toStyledString());
        m_sendData->SendMsgData(strData);
    } else {
        // 注册账号
        Json::Value root;
        root[SEND_TYPE] = DataType::eRegister;
        root[SEND_ACCOUNT_NAME] = strName.toStdString();
        root[SEND_PASSWORD] = std::string(CSEncrypt(strPwd.toStdString().c_str()));
        root[SEND_MAC] = strMac;
        std::string* strData = new std::string(root.toStyledString());
        m_sendData->SendMsgData(strData);
    }
}


void IMSClient::onResult_VerifyUserInfo(bool bret, QString msg)
{
    if (m_entryPage) {
        m_entryPage->dealWith_LoginReturn(bret, msg);
    }
}

void IMSClient::onResult_BroadcastMsg(std::string account, std::string name, bool online)
{
    if (account.empty() || name.empty()) {
        return;
    }
    bool isNew(true);
    for (auto vec : m_vecClientInfos) {
        if (vec._account.compare(account) == 0 && vec._name.compare(name) == 0) {
            isNew = false;
            break;
        }
    }
    //收到新用户登录消息
    if (isNew) {
        ClientInfo info(account, name, online);
        m_vecClientInfos.push_back(info);
        emit UpdateUserInfo(true);
    }
}

void IMSClient::onResult_TranspondMsg(std::string account, std::string name, std::string msg)
{
    std::string msgTmp = name + _QS("：").toStdString() + msg;
    m_vecClientMsgInfos.push_back(msgTmp);
    emit UpdateUserMsg();

    QString accountTmp = QString::fromStdString(account);
    do
    {
        if (m_currentSendMsgAccount.compare(accountTmp) == 0)
        {
            break;
        }
        if (!m_currentSendMsgAccount.isEmpty())
        {
            break;
        }
        //有消息过来自动跳转到消息页
        QString qname = QString::fromStdString(name);
        ui.label_currentUser->setText(qname);
        m_currentSendMsgAccount = QString::fromStdString(account);
    } while (false);
    ui.tabWidget_main->setCurrentIndex(0);
}

void IMSClient::onResult_RegisterAccountId(bool bret, QString accountId)
{
    g_ConfigPtr.SetConfigValue(CONFIG_BASE, CONFIG_ACCOUNT, accountId.toStdString().c_str());
    if (m_entryPage) {
        m_entryPage->dealWith_RegisterReturn(bret, accountId);
    }
}

void IMSClient::onReceive_ConnectServer(bool bRet)
{
    QMessageBox::warning(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("无法连接服务器，请稍后再试！"), QString::fromLocal8Bit("确定"));

    if (m_entryPage) {
        m_entryPage->close();
        delete m_entryPage;
        m_entryPage = nullptr;
    }

}

void IMSClient::updateTableView_userData(bool bUpdate)
{
    ui.tableWidget_userData->clearContents();
    if (m_vecClientInfos.empty()) {
        return;
    }
    int i = 0;

    ui.tableWidget_userData->setRowCount(m_vecClientInfos.size());
    QTableWidgetItem* itemData = nullptr;
    for (auto info : m_vecClientInfos) {
        itemData = new QTableWidgetItem(QString::fromStdString(info._name));
        itemData->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui.tableWidget_userData->setItem(i, 0, itemData);
        itemData = new QTableWidgetItem(info._online ? QString::fromLocal8Bit("在线") : QString::fromLocal8Bit("离线"));
        itemData->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui.tableWidget_userData->setItem(i, 1, itemData);
        itemData = new QTableWidgetItem(QString::fromStdString(info._account));
        itemData->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui.tableWidget_userData->setItem(i, 2, itemData);
        i++;
    }

}

void IMSClient::updateTableView_userMsg()
{
    ui.tableWidget_msgData->clearContents();
    if (m_vecClientMsgInfos.empty()) {
        return;
    }

    int row = 0;
    QTableWidgetItem* itemData = nullptr;

    ui.tableWidget_msgData->setRowCount(m_vecClientMsgInfos.size());
    for (auto mm : m_vecClientMsgInfos) {
        itemData = new QTableWidgetItem(QString::fromStdString(mm));
        itemData->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.tableWidget_msgData->setItem(row, 0, itemData);
        row++;
    }
}

void IMSClient::onUserData_itemDoubleClicked(QTableWidgetItem* item)
{
    if (item == nullptr) {
        return;
    }
    QString name = item->text();
    int row = item->row();
    ui.label_currentUser->setText(name);

    QTableWidgetItem* item2 = ui.tableWidget_userData->item(row, 2);
    if (item2) {
        m_currentSendMsgAccount = item2->text();
    }
    //m_vecClientMsgInfos.clear();
    ui.tabWidget_main->setCurrentIndex(0);
}

void IMSClient::onPushButton_sendMsgClick(bool checked)
{
    QString currentDstName = ui.label_currentUser->text();
    QString sendMsg = ui.lineEdit_sendData->text();
    do {
        if (m_currentSendMsgAccount.isEmpty()) {
            break;
        }
        if (currentDstName.isEmpty()) {
            break;
        }
        if (sendMsg.isEmpty()) {
            break;
        }

        if (m_sendData == nullptr) {
            break;
        }
        // 发送消息
        Json::Value root;
        root[SEND_TYPE] = DataType::eTranspond;
        root[SEND_ACCOUNT_NAME] = currentDstName.toStdString();
        root[SEND_ACCOUNT] = m_currentSendMsgAccount.toStdString();
        root[SEND_MSG] = sendMsg.toStdString();

        std::string* strData = new std::string(root.toStyledString());
        m_sendData->SendMsgData(strData);

        std::string msgTmp = sendMsg.toStdString();
        m_vecClientMsgInfos.push_back(msgTmp);
        emit UpdateUserMsg();

    } while (false);

    ui.lineEdit_sendData->clear();


}

void IMSClient::onSend_clientOnLine()
{
    if (!m_sendData) {
        return;
    }

    Json::Value root;
    root[SEND_TYPE] = DataType::eBroadcast;
    root[SEND_HEARTBEAT] = std::string("I'm on line.");
    root[SEND_ACCOUNT] = m_currentAccount;
    root[SEND_ACCOUNT_NAME] = m_currentAccountName;
    std::string* strData = new std::string(root.toStyledString());
    m_sendData->SendMsgData(strData);
}

