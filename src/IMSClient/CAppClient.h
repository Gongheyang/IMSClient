/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    CAppClient.h
 *@描述    客户端类声明
 *
 *@作者    GhY
 *@日期    2024年7月24日
 *@版本    v1.0.0
 *
 ****************************************************/
#ifndef  __CLIENT_H__
#define  __CLIENT_H__

#include<stdio.h>
#include<iostream>
#include <string>
#include <vector>
#include <list>
#include "MySocket.h"
#include "MyReceive.h"
#include <QObject>


/*
 *@描述:    客户端类
 *@作者:    GhY
 *@日期:    2024/07/24
 *@历史:
 */
class CAppClient : public QObject
{
    Q_OBJECT
public:
    CAppClient();

    ~CAppClient();

    /*
     *@brief    关联信号槽
     *@author   GhY
     *@date     2024/07/24
     */
    void InitSignal();

    /*
     *@brief    初始化
     *@author   GhY
     *@date     2024/07/24
     */
    bool initinstance();

    /*
     *@brief    退出
     *@author   GhY
     *@date     2024/07/24
     */
    bool exitinstance();

    int run();
    /*
     *@desc       发送数据
     *@param:     sdata  待传输数据
     *@return     void
     *@author     GhY
     *@date       2024/07/24
     *@version    v1.0.0
     *@history:
     */
    void SendData(std::string* sdata);

signals:
    void IsConnectServer(bool);

public:
    MySocket* m_mysocket;

    std::list<std::string*> m_sendBufs;

    bool m_exitFlag;   // 退出标志

protected:

};

#endif  //!__CLIENT_H__

