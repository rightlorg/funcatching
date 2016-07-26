#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QtNetwork>
#include <QList>

class QTcpServer;

namespace Ui {
    class Server;
}

struct Player
{
	QString playerName;
	QImage playerImage;
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

//    QTcpSocket *tcpSocketReciver;

private slots:
    void sendMessage();
    void readMessage();
    void readingController();
    void addTotalClientNum();
    void nextConnection();


};

#endif // SERVER_H
