/****************************************************
 *
 *@Copyright (c) 2024, GhY, All rights reserved.
 *@�ļ�    SetUserName.h
 *@����    ��¼ҳ
 *
 *@����    GhY
 *@����    2024��8��6��
 *@�汾    v1.0.0
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

