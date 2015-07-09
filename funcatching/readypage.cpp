#include "readypage.h"
#include "ui_readypage.h"

ReadyPage::ReadyPage(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::readyPage)
{
    ui->setupUi(this);
    mainwindow = parent;
}

ReadyPage::~ReadyPage()
{
    delete ui;
}

void ReadyPage::on_back_clicked()
{
    this->hide();
    mainwindow->showStartPage();
}
