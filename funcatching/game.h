#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QObject>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QImage>
#include <QTcpSocket>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "map.h"
#include "readypage.h"
#include "mainwindow.h"
#include <QColor>
#include <QPixmap>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
											QString mapPath, int gametype);
    ~Game();

    enum Camp
    {
        Catcher,
        Hider,
        None
    };

    enum GameType
    {
	    SinglePlayer,
	    Multiplayer
    };

private slots:
    void firstDataSubmit();

private:
//    bool genHeadPic(QImage image, Camp camp, QString playerName);
    void connectServer();
    void initBlock();
    void initPlayer();
//    QImage *getHeadPic();
//    QImage *headImage;
    QTcpSocket tcpSocket;
    Map *map;
    ReadyPage *readypage;
    quint32 nextBlockSize;
    MainWindow *mainwindow;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QString player_name;

	void initSceneBackground();
};

#endif // GAME_H
