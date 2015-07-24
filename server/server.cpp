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
    ClientSocket *socket = new ClientSocket(this);
    socket->setSocketDescriptor(socketId);
    qDebug()<<"a";
}
