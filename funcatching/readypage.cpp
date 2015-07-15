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
	mainwindow = parent;
	mapIndex = -1;		//There are no maps
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
		QMessageBox::warning(NULL, tr("警告"), tr("未发现map文件夹"),
											QMessageBox::Abort);
	}
	maps = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
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
		if (!dir.exists(maps[mapIndex]))
		{
			QMessageBox::warning(NULL, tr("警告"), tr("文件不存在"),
											QMessageBox::Abort);
			goto EXIT;
		}
		
		Game *game = new Game(this, maps[mapIndex]);
//		mainwindow->hideWidget();
//		mainwindow->showWidget();
//		this->hide();

	}
	EXIT: ;
}

void ReadyPage::back()
{
	mainwindow->showWidget();
//	this->show();
}
