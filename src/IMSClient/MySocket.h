/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    MySocket.h
 *@����    TCP Socket ���װ����
 *
 *@����    GhY
 *@����    2024��7��24��
 *@�汾    v1.0.0
 *
 ****************************************************/
#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__
#include "PublicDefine.h"
//#include "sigslot.h"
#include "CIniConfig.h"
#include <QObject>


/*
 *@����:    Socket ��
 *@����:    GhY
 *@����:    2024/07/24
 *@��ʷ:
 */
class MySocket /*: public sigslot::has_slots<>*/
    : public QObject
{
    Q_OBJECT

public:

signals :
    //typedef sigslot::signal1< Tcp_SendData* > SelectEvent;

    //SelectEvent OnSelectEvent;  // ��Ϣ�¼�

    void OnSendDataEvent(Tcp_SendData*);

public:

    MySocket(std::string ip = LOCAL_HOST, int port = PORT, u_long uctl = 1);

    ~MySocket();

    /*
     *@desc       ��ʼ�������Ip��Port����
     *@param:     ip  port
     *@return
     *@author     GhY
     *@date       2024/07/24
     *@version    v1.0.0
     *@history:
     */
    void InitData(const std::string& ip, const int& port);

    /*
     *@brief    ���������������
     *@author   GhY
     *@date     2024/07/24
     */
    bool ClientConnect();

    /*
     *@brief    �ر�socket
     *@author   GhY
     *@date     2024/07/24
     */
    void Close();

    /*
     *@brief    ����TCP����״̬
     *@author   GhY
     *@date     2024/08/08
     */
    bool isSocketConnect();

    /*
     *@desc       ��������
     *@param:     sdata ����������
     *@return
     *@author     GhY
     *@date       2024/07/24
     *@version    v1.0.0
     *@history:
     */
    int SendData(const std::string& sdata);

    /*
     *@brief    ��������
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
     *@brief    ��ʼ��WINSOCK
     *@author   GhY
     *@date     2024/07/24
     */
    void InitWinsock();

private:
    std::string m_ip;
    int m_port;

    u_long m_uctl;  // ������ʽ��1=��������0=���� Ĭ��1

    SOCKET m_socket;
    SOCKET m_socketClient;
};

#endif //!__MYSOCKET_H__

