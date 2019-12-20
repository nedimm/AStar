#include "ParoValMVP.h"
#include "AppQt.hpp"

ParoValMVP::ParoValMVP(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

MVPParams ParoValMVP::_readUIParams()
{
    MVPParams params{  };
    return params;
}

void ParoValMVP::on_startPushButton_pressed()
{
    //MVPParams params = _readUIParams();
    //_appQt.readParameters(params);
    _appQt.start();
}

void ParoValMVP::on_closePushButton_pressed()
{
    _appQt.close();
}
