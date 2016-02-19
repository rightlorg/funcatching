#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "QThread"
#include "QTcpSocket"
#include "QString"
#include <QMutex>
#include <QWaitCondition>

class tcp_client : public QThread
{
    Q_OBJECT

public:
    explicit tcp_client(QObject *parent = 0);
//    ~tcp_client();
    void run();
};

#endif // TCP_CLIENT_H
