#include <QApplication>
#include <QScreen>
#include "mainwindow.h"




int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow* mainwindow = new MainWindow();

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();


    int width = screenGeometry.width() / 1.48;
    int height = screenGeometry.height() / 1.48;


    mainwindow->setMinimumSize(1252, 280);
    mainwindow->resize(width, height);
    mainwindow->show();
    return app.exec();
}