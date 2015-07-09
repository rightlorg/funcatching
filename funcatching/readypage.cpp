#include "readypage.h"
#include "ui_readypage.h"
#include <QDir>
#include <QMessageBox>
#include <QString>
#include <QListWidgetItem>

ReadyPage::ReadyPage(MainWindow *parent) :
	QWidget(parent),
	ui(new Ui::ReadyPage)
{
	ui->setupUi(this);
	mainwindow = parent;
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
	QDir dir;
	QStringList filter;
	filter << "*.map";
	dir.setNameFilters(filter);
	dir.setSorting(QDir::Time);
	if (!dir.cd("map"))
	{
		dir.mkdir("map");
		QMessageBox::warning(NULL, tr("警告"), tr("未发现map文件夹"),
											QMessageBox::Abort);
	}
	QStringList maps = dir.entryList();
	for(int i  = 0; i < maps.size(); i++)
	{
		new QListWidgetItem(maps[i], ui->listWidget);
	}
}
