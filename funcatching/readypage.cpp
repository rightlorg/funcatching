#include "readypage.h"
#include "ui_readypage.h"

ReadyPage::ReadyPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::readyPage)
{
    ui->setupUi(this);
}

ReadyPage::~ReadyPage()
{
    delete ui;
}
