#include "server.h"
#include "clientthread.h"
#include <QDebug>
server::server(QObject *parent):
    QTcpServer(parent)
{
}

server::~server()
{
}

void server::incomingConnection(int socketId)
{
//    ClientThread *socket = new ClientThread();
//    socket->setSocketDescriptor(socketId);
    qDebug()<<"connect successfully";
}
