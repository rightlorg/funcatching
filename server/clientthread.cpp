#include <QtNetwork>

#include "clientthread.h"

ClientThread::ClientThread(QObject *parent)
    : QThread(parent),
      QTcpSocket(parent)

{
    nextBlockSize = 0;
}
