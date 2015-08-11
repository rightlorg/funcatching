#include "initializemap.h"
#include "ui_initializemap.h"

initializemap::initializemap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::initializemap)
{
    ui->setupUi(this);
}

initializemap::~initializemap()
{
    delete ui;
}

void initializemap::on_posCheck_clicked()
{

}

void initializemap::on_cancelButton_clicked()
{

}

void initializemap::on_okButton_clicked()
{

}

void initializemap::on_listWidget_currentRowChanged(int currentRow)
{

}

void initializemap::on_directoreButton_clicked()
{

}
