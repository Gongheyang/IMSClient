/****************************************************
*
*@Copyright (c) 2024, GhY, All rights reserved.
*@文件    EntryPage.cpp
*@描述    登录页
*
*@作者    GhY
*@日期    2024年8月6日
*@版本    v1.0.0
*
****************************************************/
#include "EntryPage.h"
#include <QPushButton>
#include <QString>
#include <QTabBar>
#include <QMessageBox>
#include "CIniConfig.h"
#include "CEDString.h"
#include "PublicDefine.h"
#include <QPixmap>


EntryPage::EntryPage(QWidget* parent /*= Q_NULLPTR*/)
    : ui(new Ui::EntryPage)
    , m_accountName("")
    , m_accountPwd("")
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    // 获取tabWidget_LoginPage的标签栏,隐藏标签栏
    ui->tabWidget_LoginPage->tabBar()->hide();
    ui->tabWidget_LoginPage->setCurrentIndex(0);

    //QDir qDir = QDir(QApplication::applicationDirPath());
    //qDir.cdUp();
    //QString strRootPath = qDir.absolutePath();

    QString filename = QString::fromLocal8Bit(":/IMSClient/image/login.png");
    QPixmap pic;
    pic.load(filename);
    pic.scaled(ui->label_LoginPng->size(), Qt::KeepAspectRatio); //等比例缩放
    ui->label_LoginPng->setScaledContents(true);
    ui->label_LoginPng->setPixmap(pic);

    //ui->tab_Login->setStyleSheet(QString::fromLocal8Bit("QWidget#tab_Login{ border-image:url(:/IMSClient/image/login_bkg.png)}"));
    //ui->tab_Register->setStyleSheet(QString::fromLocal8Bit("QWidget#tab_Register,tab_Login{ border-image:url(:/IMSClient/image/register.png)}"));
    //ui->label_pwd->setStyleSheet(QString::fromLocal8Bit("QLabel#label_pwd { background-color: rgb(255,255, 255); border: 1px solid black; padding: 5px;}"));
    //ui->label_account->setStyleSheet(QString::fromLocal8Bit("QLabel#label_account { background-color: rgb(255,255, 255); border: 1px solid black; padding: 5px;}"));
    //ui->label_registerName->setStyleSheet(QString::fromLocal8Bit("QLabel#label_registerName { background-color: rgb(255,255, 255); border: 1px solid black; padding: 5px;}"));
    //ui->label_registerPwd->setStyleSheet(QString::fromLocal8Bit("QLabel#label_registerPwd { background-color: rgb(255,255, 255); border: 1px solid black; padding: 5px;}"));
    //ui->label_verifyPwd->setStyleSheet(QString::fromLocal8Bit("QLabel#label_verifyPwd { background-color: rgb(255,255, 255); border: 1px solid black; padding: 5px;}"));

    InitWidget();
    InitSignal();
    AutoInsertData();
}

EntryPage::~EntryPage()
{
}

void EntryPage::InitWidget()
{
    ui->lineEdit_LoginAccount->clear();
    ui->lineEdit_LoginPwd->clear();

    ui->lineEdit_RegisterName->clear();
    ui->lineEdit_RegisterPwd->clear();
    ui->lineEdit_RegisterVerify->clear();

    ui->pushButton_Login->setVisible(true);
    ui->pushButton_Register->setVisible(true);
    ui->pushButton_Back->setVisible(false);
    ui->pushButton_RegisterOk->setVisible(false);
}

void EntryPage::AutoInsertData()
{
    QString account = QString::fromStdString(g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT));
    std::string strPwd = CSDecrypt(g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_PWD).c_str());
    QString pwd = QString::fromStdString(strPwd);
    if (!account.isEmpty()) {
        ui->lineEdit_LoginAccount->setText(account);
    } else {
        return;
    }
    if (!pwd.isEmpty()) {
        ui->lineEdit_LoginPwd->setText(pwd);
    }
}

void EntryPage::InitSignal()
{
    connect(ui->pushButton_Login, &QPushButton::clicked, this, &EntryPage::onPushButton_LoginClicked);
    connect(ui->pushButton_Register, &QPushButton::clicked, this, &EntryPage::onPushButton_RegisterClicked);

    connect(ui->pushButton_Back, &QPushButton::clicked, this, &EntryPage::onPushButton_BackClicked);
    connect(ui->pushButton_RegisterOk, &QPushButton::clicked, this, &EntryPage::onPushButton_RegisterOKClicked);
}

void EntryPage::closeEvent(QCloseEvent* event)
{
    event->accept();
}

void EntryPage::onPushButton_LoginClicked(bool clicked)
{
    int index = ui->tabWidget_LoginPage->currentIndex();
    if (index == 0) {
        //登陆页
        QString strAccount = ui->lineEdit_LoginAccount->text();
        QString strPwd = ui->lineEdit_LoginPwd->text();
        if (!strAccount.isEmpty() && !strPwd.isEmpty()) {
            emit VerifyUserInfo(0, strAccount, strPwd);
        } else {
            QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请检查账号或密码！"), QString::fromLocal8Bit("确定"));
        }
    } else {
        //由注册页跳转到登陆页
        ui->tabWidget_LoginPage->setCurrentIndex(0);
        InitWidget();
    }
}

void EntryPage::onPushButton_RegisterClicked(bool clicked)
{
    //由登陆页跳转到注册页
    ui->tabWidget_LoginPage->setCurrentIndex(1);
    InitWidget();
    ui->pushButton_Login->setVisible(false);
    ui->pushButton_Register->setVisible(false);
    ui->pushButton_Back->setVisible(true);
    ui->pushButton_RegisterOk->setVisible(true);

}

void EntryPage::onPushButton_RegisterOKClicked(bool clicked)
{
    //注册
    QString strUserName = ui->lineEdit_RegisterName->text();
    m_accountName = strUserName;
    QString strPwd = ui->lineEdit_RegisterPwd->text();
    QString strVerifyPwd = ui->lineEdit_RegisterVerify->text();
    m_accountPwd = strVerifyPwd;
    if (strUserName.isEmpty() || strPwd.isEmpty() || strVerifyPwd.isEmpty()) {
        QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请检查用户名或密码！"), QString::fromLocal8Bit("确定"));
        return;
    }
    if (strPwd.compare(strVerifyPwd) != 0) {
        QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("两次密码不一致，请检查！"), QString::fromLocal8Bit("确定"));
        return;
    }

    emit VerifyUserInfo(1, strUserName, strVerifyPwd);

}

void EntryPage::onPushButton_BackClicked(bool clicked)
{
    //由注册页跳转到登陆页
    ui->tabWidget_LoginPage->setCurrentIndex(0);
    InitWidget();
}

void EntryPage::dealWith_LoginReturn(bool bRet, QString msg)
{
    if (bRet) {
        //成功登录
        this->setVisible(false);

        emit LoginResult(0, msg);

    } else {
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("账号或密码错误！"), QString::fromLocal8Bit("确定"));
    }
}

void EntryPage::dealWith_RegisterReturn(bool bRet, QString accountId)
{
    if (bRet) {
        if (!m_accountName.isEmpty()) {
            g_ConfigPtr.SetConfigValue(CONFIG_BASE, "name", m_accountName.toStdString());
        }
        if (!m_accountPwd.isEmpty()) {
            g_ConfigPtr.SetConfigValue(CONFIG_BASE, "password", std::string(CSEncrypt(m_accountPwd.toStdString().c_str())));
        }

        QString msg = QString::fromLocal8Bit("请保存好你的账号: ") + accountId;
        //成功注册账号
        QMessageBox::question(this, QString::fromLocal8Bit("账号"), msg, QString::fromLocal8Bit("确定"));

        // 跳转到登录页
        onPushButton_LoginClicked(true);
    }
}
