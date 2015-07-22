#include "server.h"
#include "ui_server.h"
#include "clientsocket.h"

server::server(QWidget *parent):
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
}
