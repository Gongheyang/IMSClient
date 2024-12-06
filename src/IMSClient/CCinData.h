/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    CCinData.h
 *@描述    获取输入数据类声明
 *
 *@作者    GhY
 *@日期    2024年7月24日
 *@版本    v1.0.0
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
 *@描述:    获取输入数据类
 *@作者:    GhY
 *@日期:    2024/07/24
 *@历史:
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

