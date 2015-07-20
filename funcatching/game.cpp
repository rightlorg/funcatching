#include "game.h"

Game::Game(ReadyPage *parent, QString mapPath):
    QObject(parent)
{
    readypage = parent;
    map = new Map(NULL, mapPath);
    if(!map->loadMap())
    {
	readypage->back();
	delete this;
    }
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

