#include "initializemap.h"
#include "ui_initializemap.h"
#include "initialmap_stackmap.h"

initializemap::initializemap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::initializemap)
{
    map_num = 0;
    initialmap_stackmap *current_stack_map = new initialmap_stackmap;
    current_stack_map->show();
    ui->stackedWidget->insertWidget(map_num,current_stack_map);
    ui->stackedWidget->setCurrentIndex(map_num);
    map_num++;
    ui->setupUi(this);
}

initializemap::~initializemap()
{
    delete ui;
}

void initializemap::on_listWidget_currentRowChanged(int currentRow)
{

}

