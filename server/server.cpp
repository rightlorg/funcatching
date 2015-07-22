#include "server.h"
#include "ui_server.h"
#include "clientThread.h"

server::server(QWidget *parent):
    QTcpServer(parent)
{
}

server::~server()
{
}

void server::incomingConnection(int socketId)
{
    ClientThread *socket = new ClientThread(socketId,this);
    socket->start();
}
