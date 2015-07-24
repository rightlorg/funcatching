#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QThread>

class ClientThread : public QThread,public QTcpSocket
{
    Q_OBJECT
public:
    ClientThread(QObject *parent = 0);
    quint16 nextBlockSize;
protected:
    void run();
};

#endif
