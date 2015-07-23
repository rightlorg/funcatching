#include <QTextStream>
#include <QCoreApplication>
#include "server.h"
#include "clientThread.h"

QTextStream cin(stdin,QIODevice::ReadOnly);
QTextStream cout(stdout,QIODevice::WriteOnly);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server Server;
    if(!Server.listen(QHostAddress::Any,2048)){
        cout<<"failed to connect to the server.\n";
    }
    return a.exec();
}
