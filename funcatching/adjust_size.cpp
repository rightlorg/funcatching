#include "adjust_size.h"
#include "ui_adjust_size.h"
#include <QDebug>
adjust_size::adjust_size(QWidget *parent,int size) :
    QDialog(parent),
    ui(new Ui::adjust_size)
{
    ui->setupUi(this);
    table_size = size;
    ui->size_slider->setValue(table_size);
    switch(table_size){
    case 0:
        ui->size_label->setText("very small");
        break;
    case 1:
        ui->size_label->setText("small");
        break;
    case 2:
        ui->size_label->setText("normal");
        break;
    case 3:
        ui->size_label->setText("big");
        break;
    case 4:
        ui->size_label->setText("very big");
        break;
    }
}

adjust_size::~adjust_size()
{
    delete ui;
}

void adjust_size::on_size_slider_valueChanged(int value)
{
    table_size = value;
    switch(value){
    case 0:
        ui->size_label->setText("very small");
        break;
    case 1:
        ui->size_label->setText("small");
        break;
    case 2:
        ui->size_label->setText("normal");
        break;
    case 3:
        ui->size_label->setText("big");
        break;
    case 4:
        ui->size_label->setText("very big");
        break;
    }
}

void adjust_size::on_canceButton_clicked()
{
    close_event();
}

void adjust_size::on_okButton_clicked()
{
    this->accept();
    close_event();
}

void adjust_size::close_event()
{
    delete ui;
    this->hide();
}
