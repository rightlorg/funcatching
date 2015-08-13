#include "initialmap_stackmap.h"
#include "ui_initialmap_stackmap.h"

initialmap_stackmap::initialmap_stackmap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initialmap_stackmap)
{
    ui->setupUi(this);
}

initialmap_stackmap::~initialmap_stackmap()
{
    delete ui;
}
