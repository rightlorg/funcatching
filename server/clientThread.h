#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QTcpSocket>
#include <QThread>

class ClientThread : public QThread, public QTcpSocket
{
        Q_OBJECT

public:
    ClientThread(int socketDescriptor, QObject *parent);
    void run();

private slots:
    void readClient();
    void firstData();

private:
    quint32 nextBlockSize;
    QTcpSocket tcpSocket;
    int socketId;
};

#endif // CLIENTSOCKET_H
