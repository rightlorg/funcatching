#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QTcpSocket>
#include <QThread>

class ClientThread : public QThread
{

public:
    ClientThread(QObject *parent = 0);
    void run();
private slots:
    void readClient();

private:
    quint32 nextBlockSize;
};

#endif // CLIENTSOCKET_H
