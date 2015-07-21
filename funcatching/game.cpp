#include "game.h"
#include "QGLWidget"

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
}

Game::~Game()
{
	delete map;
}

bool Game::genHeadPic(QImage image, Camp camp, QString playerName)
{

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
	quint32 imageSize;
	disconnect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
}

