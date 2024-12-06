/****************************************************
*
*@Copyright (c) 2024, GhY, All rights reserved.
*@�ļ�    SetUserName.cpp
*@����    ��¼ҳ
*
*@����    GhY
*@����    2024��8��6��
*@�汾    v1.0.0
*
****************************************************/
#include "SetUserName.h"
#include <QPushButton>
#include <QString>
#include <QTabBar>
#include <QMessageBox>
#include "CIniConfig.h"
#include "PublicDefine.h"


SetUserName::SetUserName(QWidget* parent /*= Q_NULLPTR*/)
    : ui(new Ui::SetUserName)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    InitWidget();
}

SetUserName::~SetUserName()
{
}

void SetUserName::InitWidget()
{
    ui->lineEdit_userName->clear();

}

void SetUserName::InitSignal()
{
    connect(ui->pushButton_Save, &QPushButton::clicked, this, &SetUserName::onPushButton_SaveClicked);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &SetUserName::onPushButton_CancelClicked);
}

void SetUserName::onPushButton_SaveClicked(bool clicked)
{
    QString strName = ui->lineEdit_userName->text();
    if (!strName.isEmpty()) {
        g_ConfigPtr.SetConfigValue(CONFIG_BASE, CONFIG_NAME, strName.toLocal8Bit().toStdString());
        this->setVisible(false);
    } else {
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�û�������Ϊ�գ�"));
    }
}

void SetUserName::onPushButton_CancelClicked(bool clicked)
{
    QString strName = ui->lineEdit_userName->text();
    if (!strName.isEmpty()) {
        int ret = QMessageBox::question(nullptr, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ȷ���˳����û���δ���棿"), QString::fromLocal8Bit("ȷ��"), QString::fromLocal8Bit("ȡ��"));
        if (0 == ret) {
            this->setVisible(false);
            InitWidget();
        }
    } else {
        int ret = QMessageBox::question(nullptr, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ȷ���˳����ã�"), QString::fromLocal8Bit("ȷ��"), QString::fromLocal8Bit("ȡ��"));
        if (0 == ret) {
            this->setVisible(false);
        }
    }
}


