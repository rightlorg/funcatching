#include "server.h"
#include "ui_server.h"
#include "clientThread.h"

server::server(QObject *parent):
    QTcpServer(parent)
{
}

server::~server()
{
}

void server::incomingConnection(int socketId)
{
    ClientThread *socket = new ClientThread(socketId,this);
//    connect(socket,SIGNAL(connected()),socket,SLOT(socket->firstData()));
    //connect(socket, SIGNAL(finished()), socket, SLOT(deleteLater()));
    socket->start();
}
