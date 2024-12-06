/****************************************************
*
*@Copyright (c) 2024, GhY, All rights reserved.
*@文件    SetUserName.cpp
*@描述    登录页
*
*@作者    GhY
*@日期    2024年8月6日
*@版本    v1.0.0
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
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("用户名不能为空！"));
    }
}

void SetUserName::onPushButton_CancelClicked(bool clicked)
{
    QString strName = ui->lineEdit_userName->text();
    if (!strName.isEmpty()) {
        int ret = QMessageBox::question(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确认退出，用户名未保存？"), QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"));
        if (0 == ret) {
            this->setVisible(false);
            InitWidget();
        }
    } else {
        int ret = QMessageBox::question(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确认退出设置？"), QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"));
        if (0 == ret) {
            this->setVisible(false);
        }
    }
}


