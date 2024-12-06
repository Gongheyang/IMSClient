/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    CCinData.h
 *@����    ��ȡ��������������
 *
 *@����    GhY
 *@����    2024��7��24��
 *@�汾    v1.0.0
 *
 ****************************************************/
#ifndef  __CCINDATA_H__
#define  __CCINDATA_H__
#include<stdio.h>
#include<iostream>
#include <string>
//#include "sigslot.h"
#include <QObject>


class  CAppClient;


/*
 *@����:    ��ȡ����������
 *@����:    GhY
 *@����:    2024/07/24
 *@��ʷ:
 */
class CCinData : public QObject
{
    Q_OBJECT
public:

    //typedef sigslot::signal1<std::string* > SendDataEvent;

    //SendDataEvent OnSendEvent;
signals :

    void SendDataEvent(std::string*);

public:

    CCinData(CAppClient* app);

    ~CCinData();

    void exit();

    void SendMsgData(std::string* str);

private:
    CAppClient* m_appClient;
    bool m_exitFlag;
};

#endif  //!__CCINDATA_H__

