/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    CAppClient.cpp
 *@描述    客户端类实现
 *
 *@作者    GhY
 *@日期    2024年7月24日
 *@版本    v1.0.0
 *
 ****************************************************/
#include "CAppClient.h"
#include <QThread>



DWORD WINAPI ClientReceiveProcess(LPVOID lpParam)
{
    CAppClient* appclient = (CAppClient*)lpParam;
    if (!appclient) {
        return 1;
    }
    //if (appclient->m_sendData) {
    //    appclient->m_sendData->SendMsgData("");
    //}

    static int nCnt = 0;
    char sendBuf[2000] = { 0 };

    int recvdata = 0;
    while (true) {
        if (appclient->m_exitFlag) {
            break;
        }
        recvdata = appclient->m_mysocket->ReceiveData();
        if (recvdata == 0) {
            Sleep(500);
        } else {
            recvdata = 0;
        }
    }
    return 0;
}


DWORD WINAPI RunSendBufProcess(LPVOID lpParam)
{
    CAppClient* appclient = (CAppClient*)lpParam;
    if (!appclient) {
        return 1;
    }
    while (true) {
        if (appclient->m_exitFlag) {
            break;
        }
        if (appclient->m_sendBufs.empty()) {
            Sleep(300);
            continue;
        }

        if (appclient->m_sendBufs.size() > 0) {
            std::string* strBuf = appclient->m_sendBufs.front();
            appclient->m_sendBufs.pop_front();
            if (appclient->m_mysocket && !strBuf->empty()) {
                appclient->m_mysocket->SendData(*strBuf);
                delete strBuf;
            }
        }
    }
    return 0;
}


CAppClient::CAppClient()
{
    m_exitFlag = false;
    m_mysocket = new MySocket();
    std::string sIp = g_ConfigPtr.getConfigValueWithKey(CONFIG_NET, CONFIG_IP);
    std::string sPort = g_ConfigPtr.getConfigValueWithKey(CONFIG_NET, CONFIG_PORT);
    int iPort = sPort.empty() ? PORT : atoi(sPort.c_str());

    m_mysocket->InitData(sIp, iPort);

    m_sendBufs.clear();

    InitSignal();
}

CAppClient::~CAppClient()
{
    //this->disconnect_all();
    exitinstance();

}

bool CAppClient::initinstance()
{
    return true;
}

bool CAppClient::exitinstance()
{
    m_exitFlag = true;
    QThread::msleep(300);
    if (m_mysocket) {
        m_mysocket->Close();
        delete m_mysocket;
        m_mysocket = nullptr;
    }
    return true;
}

int CAppClient::run()
{
    if (!m_mysocket->ClientConnect()) {
        emit IsConnectServer(false);
    }

    HANDLE hProcessIO = CreateThread(NULL, 0, ClientReceiveProcess, this, 0, NULL);
    if (hProcessIO) {
        CloseHandle(hProcessIO);
    }

    HANDLE hProcessIO2 = CreateThread(NULL, 0, RunSendBufProcess, this, 0, NULL);
    if (hProcessIO2) {
        CloseHandle(hProcessIO2);
    }

    return 0;
}

void CAppClient::SendData(std::string* sdata)
{
    std::string* tmpdata = sdata;
    if (tmpdata->empty()) {
        return;
    }
    if (m_exitFlag == true) {
        return;
    }
    m_sendBufs.push_back(tmpdata);
}

void CAppClient::InitSignal()
{
    //if (m_sendData) {
    //    m_sendData->OnSendEvent.connect(this, &CAppClient::SendData);
    //}
}


