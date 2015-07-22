#include "clientsocket.h"
#include<QTextStream>

QTextStream cinn(stdin,QIODevice::ReadOnly);
QTextStream coutt(stdout,QIODevice::WriteOnly);

ClientSocket::ClientSocket(QObject *parent) :   QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readClient()));
    connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
    coutt<<"connected successfully";
    nextBlockSize = 0;
}

void ClientSocket::readClient()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);

    if(nextBlockSize==0){
        if(bytesAvailable() < sizeof(quint32))
            return;
        in >> nextBlockSize;
    }
}
