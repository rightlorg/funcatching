#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startGame_clicked()
{
    ui->centralWidget->hide();
}

void MainWindow::on_settings_clicked()
{
    ui->centralWidget->hide();
}
