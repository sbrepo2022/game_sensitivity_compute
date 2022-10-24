#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Mouse sensitivity controller (SBstudio ©)");
    w.show();

    return a.exec();
}
