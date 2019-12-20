#pragma once

#include "ui_ParoValMVP.h"
#include "AppQt.hpp"
#include "MVPParams.hpp"


class ParoValMVP : public QMainWindow
{
    Q_OBJECT

public:
    ParoValMVP(QWidget *parent = Q_NULLPTR);

private:
    Ui::ParoValMVPClass ui;
    AppQt _appQt;


private slots:
    MVPParams _readUIParams();
    void on_startPushButton_pressed();
    void on_closePushButton_pressed();

};
