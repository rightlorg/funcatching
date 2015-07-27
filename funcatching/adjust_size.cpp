#include "adjust_size.h"
#include "ui_adjust_size.h"

adjust_size::adjust_size(QWidget *parent,int size) :
    QDialog(parent),
    ui(new Ui::adjust_size)
{
    ui->setupUi(this);
}

adjust_size::adjust_size(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adjust_size)
{
    ui->setupUi(this);
}

adjust_size::~adjust_size()
{
    delete ui;
}

void adjust_size::on_size_slider_valueChanged(int value)
{
    switch(value){
    case 0:
        ui->size_label->setText("very small");
    case 1:
        ui->size_label->setText("small");
    case 2:
        ui->size_label->setText("normal");
    case 3:
        ui->size_label->setText("big");
    case 4:
        ui->size_label->setText("very big");
    }
}
