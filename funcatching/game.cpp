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
}

Game::~Game()
{
	delete map;
}

void Game::genHeadPic(QImage image, Camp camp, QString playerName)
{

}

QImage *Game::getHeadPic(QString path)
{
	QSettings settings("Funcatching Project", "Funcatching");
	settings.beginGroup("HeadImage");
	QString headPicPath = settings.value("head_image", "").toString();
	QFile a(path);
	if(!a.exists())
	{
		return NULL;
	}
	settings.endGroup();
	QImage *headImage = new QImage(path);
	return headImage;
}

//void Game::closeEvent(QCloseEvent *event)
//{
//	int r = QMessageBox::warning(NULL,tr("Game"),
//				     tr("Do you want to quit?"),QMessageBox::Yes|QMessageBox::No);
//	if(QMessageBox::Yes==r){
//		event->accept();
//	}
//	else
//		event->ignore();
//}
