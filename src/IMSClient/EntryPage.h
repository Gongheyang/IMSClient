/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    EntryPage.h
 *@描述    登录页
 *
 *@作者    GhY
 *@日期    2024年8月6日
 *@版本    v1.0.0
 *
 ****************************************************/
#pragma once
#include <QWidget>
#include <QCloseEvent>
#include "ui_EntryPage.h"



namespace Ui
{
class EntryPage;
}

class EntryPage : public QWidget
{
    Q_OBJECT
public:
    EntryPage(QWidget* parent = Q_NULLPTR);

    ~EntryPage();

    void InitWidget();

    /*
     *@brief    如果登陆过账号密码，则自动填写
     *@author   GhY
     *@date     2024/08/10
     */
    void AutoInsertData();

    /*
     *@brief    初始化信号槽
     *@author   GhY
     *@date     2024/09/02
     */
    void InitSignal();

    /*
     *@brief    获取登录结果
     *@author   GhY
     *@date     2024/09/02
     */
    void dealWith_LoginReturn(bool bRet, QString msg);

    /*
     *@brief    获取注册结果
     *@author   GhY
     *@date     2024/09/02
     */
    void dealWith_RegisterReturn(bool bRet, QString accountId);

signals:
    void  VerifyUserInfo(int, QString, QString);

    void LoginResult(int, QString);

public slots:

    virtual void closeEvent(QCloseEvent* event);

    void onPushButton_LoginClicked(bool clicked);

    void onPushButton_RegisterClicked(bool clicked);


    void onPushButton_RegisterOKClicked(bool clicked);

    void onPushButton_BackClicked(bool clicked);

private:

    Ui::EntryPage* ui;

    QString m_accountName;
    QString m_accountPwd;


};

