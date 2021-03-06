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
#include "storing_player.h"


#define PACE 1			// It can only be 1
#define PACE_PER_SECOND 20	//move n pixels in a second
#define FPS 20

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

protected:
	bool eventFilter(QObject *object, QEvent *event);
	//	void keyPressEvent(QKeyEvent *event);
	//	void keyReleaseEvent(QKeyEvent *event);

private slots:
	void firstDataSubmit();
	void getFirst();
	void gameMenu();
	void timerUpdate();

private:
	void connectServer();
	void loadTexture();
	void movePlayer(bool up, bool down, bool left, bool right);
	void onKeyPressed(QKeyEvent *event);
	void onKeyReleased(QKeyEvent *event);

	//    void handleKeyPressed(QKeyEvent *event);

	void getHeadPic(int gametype);
	void setXPos(int x_pos);
	void setYPos(int y_pos);
	QTcpSocket tcpSocket;
	Map *map;
	ReadyPage *readypage;
	quint32 nextBlockSize;
	MainWindow *mainwindow;
	QGraphicsScene scene;
	QGraphicsView view;
	QString player_name;
	QGraphicsPixmapItem *myself;
	QPixmap myself_headImage;
	QMap<unsigned short, storing_player *>player;
	QList<QPixmap> player_headImages;
	unsigned short player_index;
	QList<QList<QPixmap> > texture;
	void initSceneBackground();
	int gameType;
	QString mapPath;
	QTimer gameTick;
	QTimer flashTick;
	QTimer moveTick;
	QTimer keyLeftTick,keyRightTick,keyDownTick,KeyUpTick;
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
