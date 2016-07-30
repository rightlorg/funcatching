#include <QtGui>
#include "settings.h"
#include "mapeditor.h"
#include <QFileDialog>
#include <QInputDialog>

Settings::Settings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Settings)
{
	ui->setupUi(this);
}

Settings::~Settings()
{
	delete ui;
}

void Settings::on_backButton_clicked()
{
	this->close();
}

void Settings::on_setpicButton_clicked()
{
	filename = QFileDialog::getOpenFileName(this,tr("choose your image"),".",tr("png(*.png)\n"));
    QSettings settings("Funcatching Project","Funcatching");
    settings.beginGroup("HeadImage");
    settings.setValue("head_image",filename);
    settings.endGroup();
}

void Settings::on_mapButton_clicked()
{
	MapEditor *map= new MapEditor;
	this->close();
	map->show();
}

void Settings::on_setnameButton_clicked()
{
	bool ok = false;
	QSettings settings("Funcatching Project", "Funcatching");
	settings.beginGroup("Player Name");
	settings.setValue("name", QInputDialog::getText(this, tr("Enter your name"), tr("Enter your name"),
	                                    QLineEdit::Normal,settings.value("name").toString().isEmpty()?"hehe":settings.value("name").toString(), &ok));
	settings.endGroup();
}


void Settings::on_initialmapButton_clicked()
{
        initializemap *initialize_map_setttings = new initializemap;
        this->close();
        initialize_map_setttings->show();
}
