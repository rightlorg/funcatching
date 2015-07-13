#include "game.h"
#include <QMessageBox>
#include <QSettings>

Game::Game()
{

}

Game::~Game()
{

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
