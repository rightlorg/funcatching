#include "initializemap.h"
#include "ui_initializemap.h"

initializemap::initializemap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::initializemap)
{
    map_num = 0;
    initialmap_stackmap *current_stack_map = new initialmap_stackmap;
    ui->stackedWidget->insertWidget(map_num,current_stack_map);
//    ui->stackedWidget->addWidget(current_stack_map);
    ui->stackedWidget->setCurrentIndex(map_num);
    map_num++;
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
{/*
    QString filename = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.map)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr(""),
                             tr("failed to read file %1:\n%2")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);

    quint32 magic;
    in>>magic;

    if(magic!=MagicNum){
        QMessageBox::warning(this,tr("Map editor"),
                             tr("This file is mot a Map file\nPlease rechoose the edited file"));
        return;
    }*/
}

void initializemap::on_listWidget_currentRowChanged(int currentRow)
{

}

