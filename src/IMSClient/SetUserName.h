/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@文件    SetUserName.h
 *@描述    登录页
 *
 *@作者    GhY
 *@日期    2024年8月6日
 *@版本    v1.0.0
 *
 ****************************************************/
#pragma once
#include <QWidget>
#include <QCloseEvent>
#include "ui_SetUserName.h"



namespace Ui
{
class SetUserName;
}

class SetUserName : public QWidget
{
    Q_OBJECT
public:
    SetUserName(QWidget* parent = Q_NULLPTR);

    ~SetUserName();

    void InitWidget();

    void InitSignal();
signals:


public slots:

    void onPushButton_SaveClicked(bool clicked);

    void onPushButton_CancelClicked(bool clicked);



private:

    Ui::SetUserName* ui;




};

