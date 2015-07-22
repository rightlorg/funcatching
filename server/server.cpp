#include "server.h"
#include "ui_server.h"
#include "clientThread.h"

server::server(QWidget *parent):
    QTcpServer(parent)
{
    qDebug()<<"a";
}

server::~server()
{
}

void server::incomingConnection(int socketId)
{
    qDebug()<<"connect start";
    ClientThread *socket = new ClientThread(this);
    socket->setSocketDescriptor(socketId);
}
