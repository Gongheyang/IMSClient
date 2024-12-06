/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    MyReceive.h
 *@����    ��������������
 *
 *@����    GhY
 *@����    2024��7��24��
 *@�汾    v1.0.0
 *
 ****************************************************/
#ifndef  __MYRECEIVE_H__
#define __MYRECEIVE_H__
#include <QObject>
#include "MySocket.h"


/*
 *@����:    �������ݴ�����
 *@����:    GhY
 *@����:    2024/07/24
 *@��ʷ:
 */
class MyReceive : public QObject
{
    Q_OBJECT
public:
    MyReceive(MySocket* s);
    ~MyReceive();


    /*
     *@brief    �����źŲ�
     *@author   GhY
     *@date     2024/07/24
     */
    void InitSigslot();

    /*
     *@desc       ��������
     *@param:     sdata  ����������
     *@return     void
     *@author     GhY
     *@date       2024/07/24
     *@version    v1.0.0
     *@history:
     */
    void ReceiveData(Tcp_SendData* sdata);

signals:

    void verifyAccountWithPwdResult(bool, QString);

    void verifyAccountIdResult(bool, QString);

    void ReceiveBroadcastMsg(std::string, std::string, bool);

    void ReceiveTranspondMsg(std::string, std::string, std::string msg);   // account name msg

private:
    MySocket* m_mysocket;

};

#endif  //!__MYRECEIVE_H__

