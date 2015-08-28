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
#include <QGraphicsPixmapItem>
#include <QHostAddress>
#include <QColor>
#include <QPixmap>
#include <QList>
#include <QSettings>
#include <QEvent>
#include <QKeyEvent>
#include "map.h"
#include "readypage.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include "storing_player.h"


class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
                  QString mapPath, int gametype);
	void exitGame();
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
protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void firstDataSubmit();
    void getFirst();
    void gameMenu();

private:
//    bool genHeadPic(QImage image, Camp camp, QString playerName);
    void connectServer();
    void paintBlocks(int floor);
    void initPlayer(int gametype);
    void loadTexture();
    void handleKeyPressed(QKeyEvent *event);
    void getHeadPic(int gametype);
    inline void change_x_pos(int x_pos);
    inline void change_y_pos(int y_pos);
    //    QImage *headImage;
    QTcpSocket tcpSocket;
    Map *map;
    ReadyPage *readypage;
    quint32 nextBlockSize;
    MainWindow *mainwindow;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QString player_name;
    QGraphicsPixmapItem *myself;
    QPixmap myself_headImage;
    QMap<unsigned short, storing_player *>player;
    QList<QPixmap> player_headImages;
    unsigned short player_index;
    QList<QList<QPixmap> > texture;
    void initSceneBackground();
};

#endif // GAME_H
