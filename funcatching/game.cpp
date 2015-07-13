#include "game.h"

Game::Game()
{

}
Game::~Game()
{

}

void Game::readHeadImage()
{
    QSettings settings("Carrrow Inc.","Funcatching");
    headImageFile = settings.value("head_image").toString();
    QPixmap *headImage = new QPixmap;
    headImage->load(headImageFile);
}

void Game::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::warning(this,tr("Game"),
                                 tr("Do you want to quit?"),
                                 QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes==r){
        event->accept();
    }
    else
        event->ignore();
}
