/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    CAppClient.h
 *@����    �ͻ���������
 *
 *@����    GhY
 *@����    2024��7��24��
 *@�汾    v1.0.0
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
 *@����:    �ͻ�����
 *@����:    GhY
 *@����:    2024/07/24
 *@��ʷ:
 */
class CAppClient : public QObject
{
    Q_OBJECT
public:
    CAppClient();

    ~CAppClient();

    /*
     *@brief    �����źŲ�
     *@author   GhY
     *@date     2024/07/24
     */
    void InitSignal();

    /*
     *@brief    ��ʼ��
     *@author   GhY
     *@date     2024/07/24
     */
    bool initinstance();

    /*
     *@brief    �˳�
     *@author   GhY
     *@date     2024/07/24
     */
    bool exitinstance();

    int run();
    /*
     *@desc       ��������
     *@param:     sdata  ����������
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

    bool m_exitFlag;   // �˳���־

protected:

};

#endif  //!__CLIENT_H__

