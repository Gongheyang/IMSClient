/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    CCinData.cpp
 *@����    ��ȡ����������ʵ��
 *
 *@����    GhY
 *@����    2024��7��24��
 *@�汾    v1.0.0
 *
 ****************************************************/
#include "CCinData.h"
#include "CAppClient.h"



CCinData::CCinData(CAppClient* app)
    : m_appClient(app)
    , m_exitFlag(false)
{
}

CCinData::~CCinData()
{
    exit();
}



void CCinData::exit()
{
    m_exitFlag = true;
}

void CCinData::SendMsgData(std::string* str)
{

    //std::cout << "please cin message: " << std::endl;
    //std::string* sendTest = new std::string("����");
    //OnSendEvent.emit(sendTest);

    //while (true) {
    if (m_exitFlag) {
        return;
    }
    std::string* sendBuf = str; //new std::string();
    //std::cin >> sendBuf;
    //getline(std::cin, *sendBuf);
    //memcpy(sendBuf, str->c_str(), str->length());

    if (sendBuf->size() > 0) {
        //OnSendEvent.emit(sendBuf);
        emit SendDataEvent(sendBuf);
    } else {
        Sleep(500);
    }
    //}
}


