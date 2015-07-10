#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "readypage.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	statusLabel = new QLabel;
	createStatusBar();
	//statusImage = new QPixmap;
	//statusImage->load(":/image/pix2.png");
	//ui->label->setPixmap(*statusImage);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_startGame_clicked()
{
	ui->startWidget->hide();
	ReadyPage *readyPage = new ReadyPage(this);
	ui->gridLayout_main->addWidget(readyPage);
	statusLabel->setText("Select a map");
}

void MainWindow::on_settings_clicked()
{
	// ui->centralWidget->hide();
	settings *SettingsDialog = new settings;
	statusLabel->setText("general settings");
	SettingsDialog->exec();
}

void MainWindow::createStatusBar()
{
	statusLabel->setText("game loading");
	statusBar()->addWidget(statusLabel);
}

void MainWindow::showStartPage()
{
	ui->startWidget->show();
}

void MainWindow::delWidget()
{
	ui->centralWidget->close();
}
