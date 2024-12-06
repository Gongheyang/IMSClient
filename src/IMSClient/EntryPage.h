/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    EntryPage.h
 *@����    ��¼ҳ
 *
 *@����    GhY
 *@����    2024��8��6��
 *@�汾    v1.0.0
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
     *@brief    �����½���˺����룬���Զ���д
     *@author   GhY
     *@date     2024/08/10
     */
    void AutoInsertData();

    /*
     *@brief    ��ʼ���źŲ�
     *@author   GhY
     *@date     2024/09/02
     */
    void InitSignal();

    /*
     *@brief    ��ȡ��¼���
     *@author   GhY
     *@date     2024/09/02
     */
    void dealWith_LoginReturn(bool bRet, QString msg);

    /*
     *@brief    ��ȡע����
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

