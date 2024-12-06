/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    MySocket.h
 *@描述    TCP Socket 类封装声明
 *
 *@作者    GhY
 *@日期    2024年7月24日
 *@版本    v1.0.0
 *
 ****************************************************/
#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__
#include "PublicDefine.h"
//#include "sigslot.h"
#include "CIniConfig.h"
#include <QObject>


/*
 *@描述:    Socket 类
 *@作者:    GhY
 *@日期:    2024/07/24
 *@历史:
 */
class MySocket /*: public sigslot::has_slots<>*/
    : public QObject
{
    Q_OBJECT

public:

signals :
    //typedef sigslot::signal1< Tcp_SendData* > SelectEvent;

    //SelectEvent OnSelectEvent;  // 消息事件

    void OnSendDataEvent(Tcp_SendData*);

public:

    MySocket(std::string ip = LOCAL_HOST, int port = PORT, u_long uctl = 1);

    ~MySocket();

    /*
     *@desc       初始化服务端Ip，Port配置
     *@param:     ip  port
     *@return
     *@author     GhY
     *@date       2024/07/24
     *@version    v1.0.0
     *@history:
     */
    void InitData(const std::string& ip, const int& port);

    /*
     *@brief    与服务器进行连接
     *@author   GhY
     *@date     2024/07/24
     */
    bool ClientConnect();

    /*
     *@brief    关闭socket
     *@author   GhY
     *@date     2024/07/24
     */
    void Close();

    /*
     *@brief    测试TCP连接状态
     *@author   GhY
     *@date     2024/08/08
     */
    bool isSocketConnect();

    /*
     *@desc       发送数据
     *@param:     sdata 待发送数据
     *@return
     *@author     GhY
     *@date       2024/07/24
     *@version    v1.0.0
     *@history:
     */
    int SendData(const std::string& sdata);

    /*
     *@brief    接收数据
     *@author   GhY
     *@date     2024/07/24
     */
    int ReceiveData();

    void BindServer();

    void ListenServer();

    void RunServer();

protected:

private:

    /*
     *@brief    初始化WINSOCK
     *@author   GhY
     *@date     2024/07/24
     */
    void InitWinsock();

private:
    std::string m_ip;
    int m_port;

    u_long m_uctl;  // 阻塞方式：1=非阻塞，0=阻塞 默认1

    SOCKET m_socket;
    SOCKET m_socketClient;
};

#endif //!__MYSOCKET_H__

