/****************************************************
*
*@Copyright (c) 2024, GhY, All rights reserved.
*@�ļ�    MySocket.h
*@����    TCP Socket ���װʵ��
*
*@����    GhY
*@����    2024��7��24��
*@�汾    v1.0.0
*
****************************************************/
#include "MySocket.h"
#include <time.h>
#include "CIniConfig.h"
#ifdef WIN32
#include"winerror.h"
#pragma comment(lib, "ws2_32")
#include <Mstcpip.h>
#include <ws2tcpip.h>
#else
#include <netinet/tcp.h>
#endif
#include "json/reader.h"

using namespace std;


MySocket::MySocket(std::string ip, int port, u_long uctl)
    : m_ip(ip)
    , m_port(port)
    , m_uctl(uctl)
    , m_socket(NULL)
    , m_socketClient(NULL)
{
}

MySocket::~MySocket()
{
    Close();
}

void MySocket::InitData(const std::string& ip, const int& port)
{
    if (ip.empty()) {
        m_ip = LOCAL_HOST;
    } else {
        m_ip = ip;
    }
    m_port = port;
    InitWinsock();
}

void MySocket::InitWinsock()
{
    // ��ʼ��WINSOCK
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsd;
    if (WSAStartup(wVersionRequested, &wsd) != 0) {
        std::cout << ("init WSAStartup faild") << std::endl;
    }

    if (LOBYTE(wsd.wVersion) != 2 ||
            HIBYTE(wsd.wVersion) != 2) {
        WSACleanup();
        std::cout << "WSACleanup" << std::endl;
        return;
    }
}

/************************************************************************/
/*                              �ͻ���                                  */
/************************************************************************/

bool MySocket::ClientConnect()
{
    if (m_port == 0) {
        m_port = PORT;
    }
    if (m_ip.empty()) {
        m_ip = LOCAL_HOST; //���ػ�·��ַ127������һ̨�����ϲ��Ե�IP
    }
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == m_socket) {
        printf("[%s][%d] Error at socket(): %ld\n", __FUNCTION__, __LINE__, WSAGetLastError());
        WSACleanup();
        m_socket = NULL;
        return false;
    }

    int ret = ioctlsocket(m_socket, FIONBIO, &m_uctl);     //������ʽ   FIONBIO ������߽�ֹ�׽���m_socket������ģʽ  0=����
    if (SOCKET_ERROR == ret) {
        std::cout << (" set ioctlsocket faild: model") << m_uctl << std::endl;
    }

    int iOptVal = 0;
    int iOptLen = sizeof(int);
    BOOL bOptVal = true;
    int bOptLen = sizeof(BOOL);
    /*
     * ��ʼ������������getsockopt��
     * SO_KEEPALIVE������һ���׽���ѡ���ʹ�׽����ڻỰ�з���keepalive��Ϣ��
     * SO_KEEPALIVE�׽���ѡ����Ҫ��һ������ֵ���ݸ�setsockopt������
     * ���ΪTRUE, socket������Ϊ����keepalive��Ϣ;���ΪFALSE, socket������Ϊ������keepalive��Ϣ��
     * ��δ���ͨ��ʹ��getsockopt��������׽�����SO_KEEPALIVE��״̬������setsockopt������
     */
    int iResult = getsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOptVal, &iOptLen);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"getsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
    } else {
        wprintf(L"SO_KEEPALIVE Value: %ld\n", iOptVal);
    }

    iResult = setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, bOptLen);
    if (iResult == SOCKET_ERROR) {
        printf("setsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
    } else {
        printf("Set SO_KEEPALIVE: ON\n");
    }

    //iResult = getsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOptVal, &iOptLen);
    //if (iResult == SOCKET_ERROR) {
    //    wprintf(L"getsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
    //} else {
    //    wprintf(L"SO_KEEPALIVE Value: %ld\n", iOptVal);
    //}

    tcp_keepalive alive_in = { 0 };
    tcp_keepalive alive_out = { 0 };
    // keepalivetime     ��Աָ����ʱ���Ժ���Ϊ��λ���ڷ��͵�һ�� keep-alive ���ݰ�֮ǰû�л��
    // keepaliveinterval ��Աָ����δ�յ�ȷ�ϵ�����·������������������ݰ�֮��ļ�����Ժ���Ϊ��λ����
    alive_in.keepalivetime = 2 * 60 * 1000 ;          // ��ʼ�״�KeepAlive̽��ǰ��TCP�ձ�ʱ�� 2����
    alive_in.keepaliveinterval = 3 * 60 * 1000;
    alive_in.onoff = TRUE;
    unsigned long ulBytesReturn = 0;
    int nRet = WSAIoctl(m_socket, SIO_KEEPALIVE_VALS, &alive_in,
                        sizeof(alive_in), &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);

    if (nRet == SOCKET_ERROR) {
        wprintf(L"WSAIoctl SIO_KEEPALIVE_VALS failed with error:: %u\n", WSAGetLastError());
    }

    SOCKADDR_IN  addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = inet_addr(m_ip.c_str());//��ַ��
    addrSrv.sin_port = htons(m_port);//�ͷ������˵Ķ˿ںű���һ��
    ::connect(m_socket, (SOCKADDR*)&addrSrv, sizeof(addrSrv));//���ӷ������ˣ��׽��֣���ַת�������ȣ�
    //connect ���ӷ��� -1��WSAGetLastError �� ������Ϊ WSAEWOULDBLOCK��ʵ�������������ģ����Բ��ܸ��� connect �ķ���ֵ�ж�����״̬��
    //ԭ���ڷ���ֹSOCK_STREAM�׽����ϵ��� ���� �������ģ���Ϊ���뾭��һ��ʱ����ܽ������ӡ�
    Sleep(1000);
    return isSocketConnect();
}


void MySocket::Close()
{
    if (m_socket != NULL) {
        closesocket(m_socket);  //�ر��׽��֣��ͷ�Ϊ����׽��ַ������Դ
        m_socket = NULL;
    }
    WSACleanup();//��ֹ������׽��ֿ��ʹ��
}

int MySocket::SendData(const std::string& sdata)
{
    char sendBuf[SEND_DATA_LEN];
    memset(sendBuf, 0, sizeof(sendBuf));

    Json::Reader readJson;
    Json::Value root_value;
    std::string strData = sdata;
    bool retJson = readJson.parse(strData, root_value);
    if (!retJson) {
        printf_s("json parse receive data failure !!! \n");
    }
    std::string strAccount = "";
    std::string strName = "";
    int iType = root_value[SEND_TYPE].asInt();
    if (iType == DataType::eVerify) {
        strAccount = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT).c_str();
        strName = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME).c_str();
    } else if (iType == DataType::eSubmit) {
    } else if (iType == DataType::eAcquire) {
        strAccount = "";
        strName = root_value[SEND_ACCOUNT_NAME].asString();
    } else if (iType == DataType::eRegister) {
        strAccount = "";
        strName = root_value[SEND_ACCOUNT_NAME].asString();
    } else if (iType == DataType::eTranspond) {
        strAccount = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT).c_str();
        strName = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME).c_str();
    } else if (iType == DataType::eBroadcast) {
        strAccount = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT).c_str();
        strName = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME).c_str();
    } else {
        strAccount = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_ACCOUNT).c_str();
        strName = g_ConfigPtr.getConfigValueWithKey(CONFIG_BASE, CONFIG_NAME).c_str();
    }

    int ihead = sizeof(TcpHead);
    int ibody = sizeof(TcpBody);
    Tcp_SendData* pData = (Tcp_SendData*)sendBuf;
    pData->_head._node = strAccount.empty() ? 0 : atoi(strAccount.c_str());
    pData->_head._type = iType;
    pData->_head._time = (unsigned long)time(NULL);
    TcpBody tBody;
    tBody._length = sdata.length();
    memcpy(tBody._data, sdata.c_str(), sdata.length() + 1);
    memcpy(tBody._srcName, strName.c_str(), strName.length() + 1);
    memcpy(&sendBuf[ihead], &tBody, ibody);
    int sendLen = ihead + ibody;
    int ret = send(m_socket, sendBuf, sendLen, 0);

    return ret;
}

int MySocket::ReceiveData()
{
    std::string* redata = nullptr;
    sockaddr_in sin;
    int slen = sizeof(sin);
    char reBuf[SEND_DATA_LEN] = {0};
    int ret = recvfrom(m_socket, reBuf, SEND_DATA_LEN, 0, (sockaddr*)&sin, &slen);
    if (ret > 0) {
        Tcp_SendData* pData = (Tcp_SendData*)reBuf;
        //printf("%s\n", reBuf);
        //OnSelectEvent.emit(pData);

        Tcp_SendData* dataPtr = new Tcp_SendData;
        dataPtr->_head._type = pData->_head._type;
        dataPtr->_head._node = pData->_head._node;
        dataPtr->_head._time = pData->_head._time;
        dataPtr->_body._length = pData->_body._length;
        memcpy(dataPtr->_body._data, pData->_body._data, pData->_body._length);
        memcpy(dataPtr->_body._srcName, pData->_body._srcName, 32);
        emit OnSendDataEvent(dataPtr);
    }
    return ret;

}

bool MySocket::isSocketConnect()
{
    fd_set writefds;
    struct timeval timeout;

    FD_ZERO(&writefds);
    FD_SET(m_socket, &writefds);

    // ���ó�ʱʱ��Ϊ0��
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    // ���socket�Ƿ��д�����Ƿ����д���ݣ������ﲻд����
    if (select(m_socket + 1, NULL, &writefds, NULL, &timeout) > 0) {
        // �����д�������Ƿ�����ʧ��
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0) {
            return false; // ��ȡsocketѡ��ʧ��
        }
        return error == 0; // errorΪ0��ʾû�д���������Ȼ��Ч
    }
    return false; // ��ʱ��������������ʧЧ
}

/************************************************************************/
/*                              �����                                  */
/************************************************************************/

void MySocket::BindServer()
{
    /*
    // ����socket
    m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    // �󶨶˿�
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(m_port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_socket, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        OutErr("bind Failed!");
        return;
    }
    */
    return;
}


void MySocket::ListenServer()
{
    /*
    BindServer();
    // ���ü�������Ϊ200
    if (listen(m_socket, 200) != 0) {
        OutErr("listen Failed!");
        return;
    }
    */
    return;
}


void MySocket::RunServer()
{
    /*
    m_socketClient = NULL;
    LPPER_HANDLE_DATA PerHandleData;
    LPPER_IO_OPERATION_DATA PerIoData;
    while (true) {
        // �ȴ��ͻ��˽���
        //sClient = WSAAccept(sListen, NULL, NULL, NULL, 0);
        m_socketClient = accept(m_socket, 0, 0);

        int ret = ioctlsocket(m_socketClient, FIONBIO, &m_uctl);     //������ʽ   FIONBIO ������߽�ֹ�׽���m_socket������ģʽ  0=����
        if (SOCKET_ERROR == ret) {
            std::cout << (" set client ioctlsocket faild: model") << m_uctl << std::endl;
            return;
        }

        cout << endl << "Socket " << m_socketClient << "connect success!" << endl;

        PerHandleData = new PER_HANDLE_DATA();
        PerHandleData->_socket = m_socketClient;

        // ������Ŀͻ��˺���ɶ˿���ϵ����
        //CreateIoCompletionPort((HANDLE)m_socketClient, CompletionPort, (DWORD)PerHandleData, 0);

        // ����һ��Overlapped����ʹ�����Overlapped�ṹ��socketͶ�ݲ���
        PerIoData = new PER_IO_OPERATION_DATA();

        ZeroMemory(PerIoData, sizeof(PER_IO_OPERATION_DATA));
        PerIoData->DataBuf.buf = PerIoData->Buffer;
        PerIoData->DataBuf.len = DATA_BUFSIZE;

        // Ͷ��һ��WSARecv����
        DWORD Flags = 0;
        DWORD dwRecv = 0;
        WSARecv(m_socketClient, &PerIoData->DataBuf, 1, &dwRecv, &Flags, &PerIoData->Overlapped, NULL);
    }
    */
}


