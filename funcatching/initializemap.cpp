#include "initializemap.h"
#include "ui_initializemap.h"

initializemap::initializemap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::initializemap)
{
    QRegExp regExp("[1-9][0-9]{0,7}");
//    ui->columnEdit->setValidator(new QRegExpValidator(regExp));
//    ui->rowEdit->setValidator(new QRegExpValidator(regExp));

    directory_exist = false;
    position_checked = false;
    ui->setupUi(this);
}

initializemap::~initializemap()
{
    delete ui;
}

void initializemap::on_posCheck_clicked()
{
    if(!directory_exist){
        ui->posCheck->setChecked(false);
        return;
    }
    position_checked = !position_checked;
    ui->rowEdit->setEnabled(position_checked);
    ui->columnEdit->setEnabled(position_checked);
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
    QString filename = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.map)"));
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Map editor"),
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
    }
    ui->directoryLabel->setText(filename);
    ui->listWidget->addItem("new");
    directory_exist = true;
}
