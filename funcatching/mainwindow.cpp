#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include <QtGui>
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        statusLabel = new QLabel;
        createStatusBar();
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::on_startGame_clicked()
{
    ui->centralWidget->close();
    statusLabel->setText("game starting");
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
