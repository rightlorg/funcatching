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
#if 0
	filename = QFileDialog::getOpenFileName(this,tr("choose your image"),".",tr("png(*.png)"));
	QSettings imageSet("Carrow Inc.","fun catching");
	imageSet.setValue("imagePath",filename);
	QString filename;
	filename=QFileDialog::getOpenFileName(this,
					      tr("选择图像"),
					      ".",
					      tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if(filename.isEmpty())
	{
		return;
	}
	else
	{
		QImage* img=new QImage;

		if(! ( img->load(filename) ) ) //加载图像
		{
			QMessageBox::information(this,
						 tr("打开图像失败"),
						 tr("打开图像失败!"));
			delete img;
			return;
		}
		ui->label->setPixmap(QPixmap::fromImage(*img));
	}
#endif
}

void Settings::on_setnameButton_clicked()
{
	bool ok = false;
	QSettings settings("Funcatching Project", "Funcatching");
	settings.beginGroup("Player Name");
	settings.setValue("name", QInputDialog::getText(this, tr("Enter your name"), tr("Enter your name"),
											QLineEdit::Normal, tr("haha"), &ok));
	settings.endGroup();
}
