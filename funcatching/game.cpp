#include "game.h"
#include <QGLWidget>

Game::Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow, QString mapPath):
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
    scene = new QGraphicsScene(mainwindow);
    view = new QGraphicsView(scene, mainwindow);
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    //	    view->show();
    mainwindow->addviewWidget(view);
    headImage =  new QImage;
    headImage = getHeadPic();
    connectServer();
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
	readypage = parent_readypage;
	mainwindow  = parent_mainwindow;

	map = new Map(NULL, mapPath);
	if(!map->loadMap())
	{
		readypage->back();
		delete this;
	}
	scene = new QGraphicsScene(mainwindow);
	view = new QGraphicsView(scene, mainwindow);
	view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	mainwindow->addviewWidget(view);
	initSceneBackground();

	headImage =  new QImage;
	headImage = getHeadPic();

	connectServer();
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
}

Game::~Game()
{
    delete map;
}

bool Game::genHeadPic(QImage image, Camp camp, QString playerName)
{
}

void Game::initSceneBackground()
{
//	scene->setForegroundBrush(QColor(200, 255, 255));
	scene->setBackgroundBrush(QPixmap(":/image/pix3.png"));
}

QImage *Game::getHeadPic()
{
    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("HeadImage");
    QString path = settings.value("head_image", "").toString();
    QFile a(path);
    if(!a.exists())
    {
        return NULL;
    }
    settings.endGroup();
    QImage *headImage = new QImage(path);
    return headImage;
}

void Game::connectServer()
{
    tcpSocket.connectToHost("127.0.0.1",6176);
}

void Game::firstDataSubmit()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    qDebug()<<"connect successfully";

    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("Player Name");
    player_name = settings.value("name").toString();
    settings.endGroup();

    out.setVersion(QDataStream::Qt_4_8);
    out<<quint32(0)<<player_name<<headImage;

    disconnect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
}
