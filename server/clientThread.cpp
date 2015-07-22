#include "clientThread.h"
#include<QTextStream>

QTextStream cinn(stdin,QIODevice::ReadOnly);
QTextStream coutt(stdout,QIODevice::WriteOnly);

ClientThread::ClientThread(int socketId,QObject *parent)
    : QThread(parent),
      QTcpSocket(parent)
{
    qDebug()<<"connected successfully";
    firstData();
}

void ClientThread::run()
{
    if (!tcpSocket.setSocketDescriptor(socketId)) {
        emit error(tcpSocket.error());
        return;
    }
    nextBlockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}

void ClientThread::firstData()
{
    nextBlockSize = 0;

    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_4_8);
    in>>nextBlockSize;
    if(nextBlockSize>bytesAvailable())
        return ;
//    QString *playername;
    QByteArray block;
    in>>block;
}





