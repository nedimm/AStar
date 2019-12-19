#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ParoValMVP.h"

class ParoValMVP : public QMainWindow
{
    Q_OBJECT

public:
    ParoValMVP(QWidget *parent = Q_NULLPTR);

private:
    Ui::ParoValMVPClass ui;
};
