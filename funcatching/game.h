#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QObject>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QImage>
#include <QPointF>
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
#include <QTimer>
#include "map.h"
#include "readypage.h"
#include "mainwindow.h"
#include "server.h"
#include "storing_player.h"


#define PACE 1          // It can only be 1

class Game : public QObject
{
	Q_OBJECT
public:
	explicit Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
	              QString mapPathTemp, int gameTypeTemp);
	void exitGame();
	~Game();
	enum Camp{Catcher,Hider,None};
	enum GameType{SinglePlayer,Multiplayer};
	//    bool isInitOK();		//Check if readinitmap and loadmap faild in Game
	bool loadMap();
	void paintBlocks(int floor);
	void initPlayer(int gametype);
	void initGame();
	void reMoveAllBlocks();
	void connectServer();

protected:
	bool eventFilter(QObject *object, QEvent *event);
	//	void keyPressEvent(QKeyEvent *event);
	//	void keyReleaseEvent(QKeyEvent *event);

signals:
	void sigGetMap();
private slots:
	void initMultiPlayerGame();
	void getMap();
	void gameMenu();
	void timerUpdate();
	void getFirst();	//get all player's imformation
	void waitForgetFirst();
	void getTotalMapNum();


private:

	void loadTexture();
	void movePlayer(bool up, bool down, bool left, bool right);
	void whenKeyPressed(QKeyEvent *event);
	void whenKeyReleased(QKeyEvent *event);

	//    void handleKeyPressed(QKeyEvent *event);
	void firstDataSubmit();
	void getHeadPic(int gametype);
	void setXPos(int x_pos);
	void setYPos(int y_pos);
	QTcpSocket tcpSocket;
	Map *map;
	Server *server;
	ReadyPage *readypage;
	//blockSize -> getTotalMapNum();
	//nextBlockSize -> getFirst();
	qint32 nextBlockSize, blockSize;
	MainWindow *mainwindow;
	QGraphicsScene scene;
	QGraphicsView view;
	qint32 totalRemoteMapNum;
	QString player_name;
	QGraphicsPixmapItem *myself;
	QPixmap myself_headImage;
	QMap<unsigned short, storing_player *>player;
	QList<QPixmap> player_headImages;
	unsigned short player_index;
	QList<QList<QPixmap> > texture;
	QList<QPixmap> specialTex;
	void initSceneBackground();
	int floor;
	int gameType;
	QString mapPath;
	QTimer gameTick;
	bool haveWallA, haveWallB;
	QGraphicsPixmapItem collisionCheckBlock;
	//    QPointF myselfPos;

	//move
	bool finalMoveRight;
	bool finalMoveLeft;
	bool finalMoveUp;
	bool finalMoveDown;
	bool moveRight;
	bool moveLeft;
	bool moveUp;
	bool moveDown;


	//    enum Action {Left, Right, Up, Down, Pause};
	//    QMap<int, Action> actions;

	//    bool loadSuccess;
};

#endif // GAME_H
