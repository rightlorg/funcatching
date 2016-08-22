#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QtNetwork>
#include <QList>
#include "map.h"
#include "client.h"
#include <QPointF>
#include <QTimer>

#define GAME_TICK 20

class QTcpServer;

namespace Ui {
    class Server;
}

struct Player
{
	QString playerName;
	QImage playerImage;
	QTcpSocket *socket;
	QHostAddress address;
	QPointF pos;
};

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

signals:
	void currentDataExchangeDone();
	void sendTotalMapNumDone();
private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    qint32 blockSize;
    QTcpSocket *currentClient;
    int totalClientNum;
    bool readyToReadNext;
    QList<Player> playerList;
    Map *map;
    Client *sendMapClient;
    QUdpSocket *udpSocket;
    QStringList mapPath;
    QTimer timer;
    QPoint spawnPoint;
    int mapSent;
    bool loadMap();
    void sendAllPlayer();
void writeDatagrame();
//    QTcpSocket *tcpSocketReciver;

private slots:
    void sendMessage();
    void exchangeData();
    void startUdpTimer();

    void readingController();
    void addTotalClientNum();
    void processPendingDatagram();
    void nextConnection();
    void timerUpdate();
    void sendTotalMapNum();
    void sendNextMap();


    void on_pushButton_clicked();
};

#endif // SERVER_H
