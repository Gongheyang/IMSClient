#pragma once
#include <QMainWindow>
#include <QCloseEvent>
#include <vector>
#include "ui_IMSClient.h"
#include "EntryPage.h"
#include "SetUserName.h"
#include "PublicDefine.h"
#include <QTimer>

class CAppClient;
class CCinData;
class MyReceive;

class IMSClient : public QMainWindow
{
    Q_OBJECT

public:
    IMSClient(QWidget* parent = Q_NULLPTR);

    ~IMSClient();

    void InitSignal();

    void initWidget();

    bool showLoginWidget();
signals:

    void UpdateUserInfo(bool);
    void UpdateUserMsg();

public slots:

    virtual void closeEvent(QCloseEvent* event);

    void onEntryPage_LoginResult(int ret, QString str);

    void onPushButton_MessageClicked(bool clicked);

    void onPushButton_PersonsClicked(bool clicked);

    void onDeal_VerifyUserInfo(const int& type, const QString& strName, const QString& strPwd);

    void onResult_VerifyUserInfo(bool bret, QString msg);

    /*
     *@brief    获取上线广播消息
     *@author   GhY
     *@date     2024/09/02
     */
    void onResult_BroadcastMsg(std::string account, std::string name, bool online);

    void onResult_TranspondMsg(std::string account, std::string name, std::string msg);

    /*
     *@brief    获取注册账户结果信息
     *@author   GhY
     *@date     2024/09/02
     */
    void onResult_RegisterAccountId(bool bret, QString accountId);

    /*
     *@brief    获取连接服务器状态
     *@author   GhY
     *@date     2024/09/02
     */
    void onReceive_ConnectServer(bool bRet);

    void updateTableView_userData(bool bUpdate);

    void updateTableView_userMsg();

    void onUserData_itemDoubleClicked(QTableWidgetItem* item);

    void onPushButton_sendMsgClick(bool checked);
    void onSend_clientOnLine();
private:
    Ui::IMSClientClass ui;

    EntryPage* m_entryPage;
    SetUserName* m_setUserNamePtr;

    CAppClient* m_app;
    CCinData* m_sendData;
    MyReceive* m_myrev;

    std::vector<ClientInfo> m_vecClientInfos;
    std::vector<std::string> m_vecClientMsgInfos;

    std::string m_currentAccount;
    std::string m_currentAccountName;

    QTimer* m_timer;
    QString m_currentSendMsgAccount;

};
