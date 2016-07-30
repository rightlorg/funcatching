#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QtNetwork>
#include <QList>
#include "../../funcatching/map.h"


class QTcpServer;

namespace Ui {
    class Server;
}

struct Player
{
	QString playerName;
	QImage playerImage;
	QTcpSocket *socket;
};

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    quint32 blockSize;
    QTcpSocket *currentClient;
    int totalClientNum;
    bool readyToReadNext;
    QList<Player> playerList;
    Map *map;
    bool loadMap();

//    QTcpSocket *tcpSocketReciver;

private slots:
    void sendMessage();
    void exchangeData();
    void readingController();
    void addTotalClientNum();
    void nextConnection();


};

#endif // SERVER_H
