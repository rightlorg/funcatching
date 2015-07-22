#include "clientThread.h"
#include<QTextStream>

QTextStream cinn(stdin,QIODevice::ReadOnly);
QTextStream coutt(stdout,QIODevice::WriteOnly);

ClientThread::ClientThread(QObject *parent) : QThread(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readClient()));
    connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
    coutt<<"connected successfully";
    nextBlockSize = 0;
}

void ClientThread::readClient()
{
    qDebug()<<"asdfad";
//    QDataStream in(this);
//    in.setVersion(QDataStream::Qt_4_8);

//    if(nextBlockSize==0){
//        if(bytesAvailable() < sizeof(quint32))
//            return;
//        in >> nextBlockSize;
//    }
}
