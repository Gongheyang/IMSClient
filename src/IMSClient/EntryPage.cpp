/****************************************************
*
*@Copyright (c) 2024, GhY, All rights reserved.
*@�ļ�    EntryPage.cpp
*@����    ��¼ҳ
*
*@����    GhY
*@����    2024��8��6��
*@�汾    v1.0.0
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

    // ��ȡtabWidget_LoginPage�ı�ǩ��,���ر�ǩ��
    ui->tabWidget_LoginPage->tabBar()->hide();
    ui->tabWidget_LoginPage->setCurrentIndex(0);

    //QDir qDir = QDir(QApplication::applicationDirPath());
    //qDir.cdUp();
    //QString strRootPath = qDir.absolutePath();

    QString filename = QString::fromLocal8Bit(":/IMSClient/image/login.png");
    QPixmap pic;
    pic.load(filename);
    pic.scaled(ui->label_LoginPng->size(), Qt::KeepAspectRatio); //�ȱ�������
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
        //��½ҳ
        QString strAccount = ui->lineEdit_LoginAccount->text();
        QString strPwd = ui->lineEdit_LoginPwd->text();
        if (!strAccount.isEmpty() && !strPwd.isEmpty()) {
            emit VerifyUserInfo(0, strAccount, strPwd);
        } else {
            QMessageBox::question(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�����˺Ż����룡"), QString::fromLocal8Bit("ȷ��"));
        }
    } else {
        //��ע��ҳ��ת����½ҳ
        ui->tabWidget_LoginPage->setCurrentIndex(0);
        InitWidget();
    }
}

void EntryPage::onPushButton_RegisterClicked(bool clicked)
{
    //�ɵ�½ҳ��ת��ע��ҳ
    ui->tabWidget_LoginPage->setCurrentIndex(1);
    InitWidget();
    ui->pushButton_Login->setVisible(false);
    ui->pushButton_Register->setVisible(false);
    ui->pushButton_Back->setVisible(true);
    ui->pushButton_RegisterOk->setVisible(true);

}

void EntryPage::onPushButton_RegisterOKClicked(bool clicked)
{
    //ע��
    QString strUserName = ui->lineEdit_RegisterName->text();
    m_accountName = strUserName;
    QString strPwd = ui->lineEdit_RegisterPwd->text();
    QString strVerifyPwd = ui->lineEdit_RegisterVerify->text();
    m_accountPwd = strVerifyPwd;
    if (strUserName.isEmpty() || strPwd.isEmpty() || strVerifyPwd.isEmpty()) {
        QMessageBox::question(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�����û��������룡"), QString::fromLocal8Bit("ȷ��"));
        return;
    }
    if (strPwd.compare(strVerifyPwd) != 0) {
        QMessageBox::question(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������벻һ�£����飡"), QString::fromLocal8Bit("ȷ��"));
        return;
    }

    emit VerifyUserInfo(1, strUserName, strVerifyPwd);

}

void EntryPage::onPushButton_BackClicked(bool clicked)
{
    //��ע��ҳ��ת����½ҳ
    ui->tabWidget_LoginPage->setCurrentIndex(0);
    InitWidget();
}

void EntryPage::dealWith_LoginReturn(bool bRet, QString msg)
{
    if (bRet) {
        //�ɹ���¼
        this->setVisible(false);

        emit LoginResult(0, msg);

    } else {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�˺Ż��������"), QString::fromLocal8Bit("ȷ��"));
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

        QString msg = QString::fromLocal8Bit("�뱣�������˺�: ") + accountId;
        //�ɹ�ע���˺�
        QMessageBox::question(this, QString::fromLocal8Bit("�˺�"), msg, QString::fromLocal8Bit("ȷ��"));

        // ��ת����¼ҳ
        onPushButton_LoginClicked(true);
    }
}
