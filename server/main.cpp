#include "server_start.h"
#include <server.h>
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    server_start w;
    w.show();
    server Server;
    if(!Server.listen(QHostAddress::Any,2048)){
        return 0;
    }

    return a.exec();
}
