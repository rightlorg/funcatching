#include "readypage.h"
#include "ui_readypage.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QModelIndex>
#include <QDebug>
#include <game.h>


ReadyPage::ReadyPage(MainWindow *parent) :
	QWidget(parent),
	ui(new Ui::ReadyPage)
{
	ui->setupUi(this);
	ip = "";
	game = NULL;
	mainwindow = parent;
	mapIndex = -1;		//There are no maps
	game = NULL;
	addMap();
}

ReadyPage::~ReadyPage()
{
	delete ui;
}

void ReadyPage::on_back_clicked()
{
	mainwindow->showStartPage();
	this->close();
}

void ReadyPage::addMap()
{
	dir.setSorting(QDir::Time);
	if (!dir.cd("map"))
	{
		dir.mkdir("map");
		QMessageBox::warning(NULL, tr("警告"), tr("未发现map文件夹，已在当前目录新建"),
											QMessageBox::Abort);
		return;
	}
	maps = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
	for(int i  = 0; i < maps.size(); i++)
	{
		new QListWidgetItem(maps[i], ui->listWidget);
	}
}

void ReadyPage::on_listWidget_clicked(const QModelIndex &index)
{
	mapIndex = index.row();
}

void ReadyPage::on_go_clicked()
{
    if (mapIndex != -1)
	{
	    if (!haveHeadImage()) {
		    QMessageBox::warning(NULL, tr("警告"), tr("No head image"),
		                                        QMessageBox::Abort);
		    return;

	    }
		if (!dir.exists(maps[mapIndex]))
		{
			QMessageBox::warning(NULL, tr("警告"), tr("文件不存在"),
			                                QMessageBox::Abort);
			return;
		}
		this->hide();
		Game *gameTemp = new Game(this, mainwindow, dir.absoluteFilePath(maps[mapIndex]), Game::SinglePlayer);
		if (!gameTemp->loadMap()) {
			delete gameTemp;
			this->show();
		} else {
			gameTemp->initGame();
		}
	}
}

void ReadyPage::back()
{
//	mainwindow->showWidget();
//	delete game;
	this->show();
}

void ReadyPage::on_server_clicked()
{
	bool ok = false;
	QSettings settings("Funcatching Project", "Funcatching");
	settings.beginGroup("IP Address");
	ip = QInputDialog::getText(this, tr("Enter IP"), tr("Enter IP"),
								QLineEdit::Normal,
								settings.value("IP").toString().isEmpty()?"127.0.0.1":settings.value("IP").toString(),
								&ok);
	settings.setValue("IP", ip);
	settings.endGroup();
	if (ok) {
			this->hide();
			Game *gameTemp = new Game(this, mainwindow, "", Game::Multiplayer);
			if (gameTemp == NULL)
				this->show();
			gameTemp->connectServer();
			connect(gameTemp, SIGNAL(sigGetFirst()), this, SLOT(startRemoteGame()));
			game = reinterpret_cast<int *>(gameTemp);

	}
}

void ReadyPage::startRemoteGame()
{
	Game *gameTemp = reinterpret_cast<Game *>(game);
	if (!gameTemp->loadMap()) {
		delete gameTemp;
		game = NULL;
		this->show();
	} else {
		gameTemp->initGame();
	}
}

bool ReadyPage::haveHeadImage()
{
	QSettings settings("Funcatching Project", "Funcatching");
	settings.beginGroup("HeadImage");
	QString path = settings.value("head_image", "").toString();
	QFile a(path);
	settings.endGroup();

	if(!a.exists())
		return false;
	return true;
}
