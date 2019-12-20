#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "ParoValMVP.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    ParoValMVP parovalMain;
    parovalMain.show();
    return qApplication.exec();
}
