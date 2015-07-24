#include "server_start.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        qDebug()<<"w";
    server_start w;
    w.show();

    return a.exec();
}
