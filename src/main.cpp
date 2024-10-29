#include <QApplication>
#include "mainwindow.h"




int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow* mainwindow = new MainWindow();

    mainwindow->setMinimumSize(740, 480);
    mainwindow->resize(1280, 760);
    mainwindow->show();


    return app.exec();
}