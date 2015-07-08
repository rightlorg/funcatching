#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *w = new MainWindow;
    w->show();
    return app.exec();
}
