#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QtGui>
#include <QTcpSocket>
class ClientSocket : public QTcpSocket
{
    //Q_OBJECT
public:
    ClientSocket();
};

#endif // CLIENTSOCKET_H
