#include "gamemenu.h"
#include "ui_gamemenu.h"

GameMenu::GameMenu(QWidget *parent, Game *gameP) :
	QDialog(parent),
	ui(new Ui::GameMenu)
{
	ui->setupUi(this);
	game = gameP;
}

GameMenu::~GameMenu()
{
	delete ui;
}

void GameMenu::on_exitButton_clicked()
{
	game->exitGame();
	delete this;
}

void GameMenu::on_pushButton_clicked()
{

}
