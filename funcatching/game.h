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
#include "map.h"
#include "readypage.h"
#include "mainwindow.h"
#include "storing_player.h"


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
    void getFirst();

private:
//	    bool genHeadPic(QImage image, Camp camp, QString playerName);
	void connectServer();
	void initBlock();
	void initPlayer();
	void loadTexture();
	void getHeadPic();
	//    QImage *headImage;
	QTcpSocket tcpSocket;
	Map *map;
	ReadyPage *readypage;
	quint32 nextBlockSize;
	MainWindow *mainwindow;
	QGraphicsScene *scene;
	QGraphicsView *view;
	QString player_name;
	QImage *headImage;
    QMap<unsigned short, storing_player *>player;
    QVector<QImage*>player_headImages;
    unsigned short player_index = 0;
	QList<QList<QPixmap> > texture;
	void initSceneBackground();
};

#endif // GAME_H
