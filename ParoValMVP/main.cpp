#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "MVPMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);
    MVPMainWindow mainWindow;
    mainWindow.show();
    return qApplication.exec();
}
