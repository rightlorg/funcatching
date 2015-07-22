#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QTcpSocket>
#include <QThread>
//struct ClientStruct{
//    //QImage headImage;
//    QString playername;
//};

class ClientThread : public QThread, public QTcpSocket
{

public:
    ClientThread(int socketDescriptor, QObject *parent);
    void run();
    void firstData();
private slots:
    void readClient();

private:
    quint32 nextBlockSize;
    QTcpSocket tcpSocket;
    int socketId;
};

#endif // CLIENTSOCKET_H
