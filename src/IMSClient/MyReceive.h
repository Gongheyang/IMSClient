/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    MyReceive.h
 *@描述    处理数据类声明
 *
 *@作者    GhY
 *@日期    2024年7月24日
 *@版本    v1.0.0
 *
 ****************************************************/
#ifndef  __MYRECEIVE_H__
#define __MYRECEIVE_H__
#include <QObject>
#include "MySocket.h"


/*
 *@描述:    接收数据处理类
 *@作者:    GhY
 *@日期:    2024/07/24
 *@历史:
 */
class MyReceive : public QObject
{
    Q_OBJECT
public:
    MyReceive(MySocket* s);
    ~MyReceive();


    /*
     *@brief    关联信号槽
     *@author   GhY
     *@date     2024/07/24
     */
    void InitSigslot();

    /*
     *@desc       接收数据
     *@param:     sdata  待接收数据
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

