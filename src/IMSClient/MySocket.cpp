/****************************************************
*
*@Copyright (c) 2024, GhY, All rights reserved.
*@文件    MySocket.h
*@描述    TCP Socket 类封装实现
*
*@作者    GhY
*@日期    2024年7月24日
*@版本    v1.0.0
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
    // 初始化WINSOCK
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
/*                              客户端                                  */
/************************************************************************/

bool MySocket::ClientConnect()
{
    if (m_port == 0) {
        m_port = PORT;
    }
    if (m_ip.empty()) {
        m_ip = LOCAL_HOST; //本地回路地址127，用于一台机器上测试的IP
    }
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == m_socket) {
        printf("[%s][%d] Error at socket(): %ld\n", __FUNCTION__, __LINE__, WSAGetLastError());
        WSACleanup();
        m_socket = NULL;
        return false;
    }

    int ret = ioctlsocket(m_socket, FIONBIO, &m_uctl);     //阻塞方式   FIONBIO 允许或者禁止套接字m_socket非阻塞模式  0=阻塞
    if (SOCKET_ERROR == ret) {
        std::cout << (" set ioctlsocket faild: model") << m_uctl << std::endl;
    }

    int iOptVal = 0;
    int iOptLen = sizeof(int);
    BOOL bOptVal = true;
    int bOptLen = sizeof(BOOL);
    /*
     * 初始化变量并调用getsockopt。
     * SO_KEEPALIVE参数是一个套接字选项，它使套接字在会话中发送keepalive消息。
     * SO_KEEPALIVE套接字选项需要将一个布尔值传递给setsockopt函数。
     * 如果为TRUE, socket被配置为发送keepalive消息;如果为FALSE, socket被配置为不发送keepalive消息。
     * 这段代码通过使用getsockopt函数检查套接字上SO_KEEPALIVE的状态来测试setsockopt函数。
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
    // keepalivetime     成员指定超时（以毫秒为单位）在发送第一个 keep-alive 数据包之前没有活动。
    // keepaliveinterval 成员指定在未收到确认的情况下发送连续保持连接数据包之间的间隔（以毫秒为单位）。
    alive_in.keepalivetime = 2 * 60 * 1000 ;          // 开始首次KeepAlive探测前的TCP空闭时间 2分钟
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
    addrSrv.sin_addr.S_un.S_addr = inet_addr(m_ip.c_str());//地址，
    addrSrv.sin_port = htons(m_port);//和服务器端的端口号保持一致
    ::connect(m_socket, (SOCKADDR*)&addrSrv, sizeof(addrSrv));//连接服务器端（套接字，地址转换，长度）
    //connect 连接返回 -1，WSAGetLastError 中 错误码为 WSAEWOULDBLOCK，实际连接是正常的；所以不能根据 connect 的返回值判断连接状态。
    //原因：在非阻止SOCK_STREAM套接字上调用 连接 是正常的，因为必须经过一段时间才能建立连接。
    Sleep(1000);
    return isSocketConnect();
}


void MySocket::Close()
{
    if (m_socket != NULL) {
        closesocket(m_socket);  //关闭套接字，释放为这个套接字分配的资源
        m_socket = NULL;
    }
    WSACleanup();//终止对这个套接字库的使用
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

    // 设置超时时间为0秒
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    // 检查socket是否可写，即是否可以写数据，但这里不写数据
    if (select(m_socket + 1, NULL, &writefds, NULL, &timeout) > 0) {
        // 如果可写，则检查是否连接失败
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0) {
            return false; // 获取socket选项失败
        }
        return error == 0; // error为0表示没有错误，连接仍然有效
    }
    return false; // 超时或其他错误，连接失效
}

/************************************************************************/
/*                              服务端                                  */
/************************************************************************/

void MySocket::BindServer()
{
    /*
    // 创建socket
    m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    // 绑定端口
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
    // 设置监听队列为200
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
        // 等待客户端接入
        //sClient = WSAAccept(sListen, NULL, NULL, NULL, 0);
        m_socketClient = accept(m_socket, 0, 0);

        int ret = ioctlsocket(m_socketClient, FIONBIO, &m_uctl);     //阻塞方式   FIONBIO 允许或者禁止套接字m_socket非阻塞模式  0=阻塞
        if (SOCKET_ERROR == ret) {
            std::cout << (" set client ioctlsocket faild: model") << m_uctl << std::endl;
            return;
        }

        cout << endl << "Socket " << m_socketClient << "connect success!" << endl;

        PerHandleData = new PER_HANDLE_DATA();
        PerHandleData->_socket = m_socketClient;

        // 将接入的客户端和完成端口联系起来
        //CreateIoCompletionPort((HANDLE)m_socketClient, CompletionPort, (DWORD)PerHandleData, 0);

        // 建立一个Overlapped，并使用这个Overlapped结构对socket投递操作
        PerIoData = new PER_IO_OPERATION_DATA();

        ZeroMemory(PerIoData, sizeof(PER_IO_OPERATION_DATA));
        PerIoData->DataBuf.buf = PerIoData->Buffer;
        PerIoData->DataBuf.len = DATA_BUFSIZE;

        // 投递一个WSARecv操作
        DWORD Flags = 0;
        DWORD dwRecv = 0;
        WSARecv(m_socketClient, &PerIoData->DataBuf, 1, &dwRecv, &Flags, &PerIoData->Overlapped, NULL);
    }
    */
}


