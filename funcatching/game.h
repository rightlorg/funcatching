#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QObject>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QImage>
#include <QTcpSocket>
#include "map.h"
#include "readypage.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(ReadyPage *parent, QString mapPath);
    ~Game();

    enum Camp
    {
        Catcher,
        Hider,
        None
    };

private slots:
    void firstDataSubmit();

private:
    bool genHeadPic(QImage image, Camp camp, QString playerName);
    void connectServer();
    QImage *getHeadPic();
    QImage *headImage;
    QTcpSocket tcpSocket;
    Map *map;
    ReadyPage *readypage;
    quint32 nextBlockSize;
};

#endif // GAME_H
