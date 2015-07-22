#include "game.h"
#include <QGLWidget>

Game::Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
										QString mapPath, int gametype):
    QObject(parent_readypage)
{
	readypage = parent_readypage;
	mainwindow  = parent_mainwindow;

	map = new Map(NULL, mapPath);
	if(!map->loadMap())
	{
		readypage->back();
		delete this;
	}

//	scene = new QGraphicsScene(mainwindow);
//	view = new QGraphicsView(scene, mainwindow);
//	view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
//	mainwindow->addviewWidget(view);

//	initSceneBackground();

//	if(gametype == SinglePlayer) {
//		initBlock();
//		initPlayer();
//	} else {
//		connectServer();
//		connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
//	}
    connectServer();
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
}

Game::~Game()
{
    delete map;
}

//bool Game::genHeadPic(QImage image, Camp camp, QString playerName)
//{
//}

void Game::initSceneBackground()
{
//	scene->setForegroundBrush(QColor(200, 255, 255));
	scene->setBackgroundBrush(QPixmap(":/image/pix3.png"));
	scene->setSceneRect(-100, -100, 200, 200);
}

void Game::getHeadPic()
{
    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("HeadImage");
    QString path = settings.value("head_image", "").toString();
    QFile a(path);
    if(!a.exists())
    {
        return;
    }
    settings.endGroup();
    headImage =  new QImage;
    headImage->load(path);
}

void Game::connectServer()
{
    tcpSocket.connectToHost(QHostAddress::LocalHost,2048);
    qDebug()<<"sta";
}

void Game::initBlock()
{

}

void Game::initPlayer()
{

}

void Game::firstDataSubmit()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("Player Name");
    player_name = settings.value("name").toString();
    settings.endGroup();

    out.setVersion(QDataStream::Qt_4_8);
    out<<quint32(0)<<player_name<<headImage;
    out.device()->seek(0);
    out<<quint32(block.size()-sizeof(quint16));
    tcpSocket.write(block);

    disconnect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
}

void initHeadPic()
{

}
