#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class server : public QTcpServer
{
    Q_OBJECT
public:
    explicit server(QObject *parent = 0);
    ~server();

private:
    void incomingConnection(int socketId);
};

#endif // SERVER_H
