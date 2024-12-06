/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    MyReceive.cpp
 *@描述    处理数据类实现
 *
 *@作者    GhY
 *@日期    2024年7月24日
 *@版本    v1.0.0
 *
 ****************************************************/
#include "MyReceive.h"
#include "json/reader.h"



MyReceive::MyReceive(MySocket* s)
    : m_mysocket(s)
{
    InitSigslot();
}


MyReceive::~MyReceive()
{
}

void MyReceive::InitSigslot()
{
    if (m_mysocket) {
        connect(m_mysocket, &MySocket::OnSendDataEvent, this, &MyReceive::ReceiveData);
    }
}


void MyReceive::ReceiveData(Tcp_SendData* sdata)
{
    if (!sdata) {
        return;
    }
    do {
        if (sdata->_head._type == DataType::eSubmit) {
        } else if (sdata->_head._type == DataType::eTranspond) {
            std::string sendName = sdata->_body._srcName;
            std::string sendAccount = std::to_string(sdata->_head._node);
            std::string sendMsg = "";
            std::string receiveData = sdata->_body._data;
            Json::Reader readJson;
            Json::Value root_value;
            bool ret = readJson.parse(receiveData, root_value);
            if (!ret) {
                printf_s("json parse receive data failure !!! \n");
            } else {
                sendMsg = root_value[SEND_MSG].asString();
            }
            emit ReceiveTranspondMsg(sendAccount, sendName, sendMsg);
            break;
        } else if (sdata->_head._type == DataType::eAcquire) {
            std::string tmp = sdata->_body._data;
            g_ConfigPtr.SetConfigValue(CONFIG_BASE, CONFIG_ACCOUNT, tmp);
        } else if (sdata->_head._type == DataType::eRegister) {
            bool bReturn = false;
            std::string  accountId = "";
            std::string receiveData = sdata->_body._data;
            Json::Reader readJson;
            Json::Value root_value;
            bool ret = readJson.parse(receiveData, root_value);
            if (!ret) {
                printf_s("json parse receive data failure !!! \n");
            } else {
                bReturn = root_value[SEND_RESULT].asBool();
                accountId = root_value[SEND_ACCOUNT].asString();
            }
            emit verifyAccountIdResult(bReturn, QString::fromStdString(accountId));
            break;
        } else if (sdata->_head._type == DataType::eVerify) {

            bool bResult = false;
            std::string retMsg = "";
            std::string receiveData = sdata->_body._data;
            Json::Reader readJson;
            Json::Value root_value;
            bool ret = readJson.parse(receiveData, root_value);
            if (!ret) {
                printf_s("json parse receive data failure !!! \n");
            } else {
                bResult = root_value[SEND_VERIFY].asBool();
                retMsg = root_value[SEND_MSG].asString();
            }

            emit verifyAccountWithPwdResult(bResult, QString::fromStdString(retMsg));
            break;
        } else if (sdata->_head._type == DataType::eBroadcast) {
            std::string receiveData = sdata->_body._data;
            std::string strAccount = "";
            std::string strName = "";
            Json::Reader readJson;
            Json::Value root_value;
            bool ret = readJson.parse(receiveData, root_value);
            if (!ret) {
                printf_s("json parse receive data failure !!! \n");
            } else {
                strAccount = root_value[SEND_ACCOUNT].asString();
                strName = root_value[SEND_ACCOUNT_NAME].asString();
            }
            emit ReceiveBroadcastMsg(strAccount, strName, true);
            break;
        } else if (sdata->_head._type == DataType::eExit) {
            std::string receiveData = sdata->_body._data;
            std::string strAccount = "";
            std::string strName = "";
            Json::Reader readJson;
            Json::Value root_value;
            bool ret = readJson.parse(receiveData, root_value);
            if (!ret) {
                printf_s("json parse receive data failure !!! \n");
            } else {
                strAccount = root_value[SEND_ACCOUNT].asString();
                strName = root_value[SEND_ACCOUNT_NAME].asString();
            }
            emit ReceiveBroadcastMsg(strAccount, strName, false);
        } else {
            std::string tmpName = sdata->_body._srcName;
            std::string tmp = sdata->_body._data;
            std::cout << "send: " << tmpName.c_str() << " -- message:  " << tmp.c_str() << std::endl;
        }
    } while (0);

    delete sdata;
    sdata = nullptr;
}

