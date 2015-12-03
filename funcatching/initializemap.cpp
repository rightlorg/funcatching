#include "initializemap.h"
#include "ui_initializemap.h"
#include "initialmap_stackmap.h"

initializemap::initializemap(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::initializemap)
{
//    pos_checked = false;
    map_num = 0;
    ui->setupUi(this);

    initialmap_stackmap *current_stack_map = new initialmap_stackmap;
    ui->stackedWidget->insertWidget(map_num,current_stack_map);
    ui->stackedWidget->setCurrentIndex(map_num);
    total_stack_num.insert(map_num,current_stack_map);
    connect(current_stack_map->ui->directoryButton,SIGNAL(clicked()),this,SLOT(on_currentstack_directory_clicked()));
}

initializemap::~initializemap()
{
    delete ui;
}

void initializemap::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void initializemap::on_currentstack_directory_clicked()
{
    ui->listWidget->currentItem()->setText(QString::number(++map_num));
    initialmap_stackmap *current_stack_map = new initialmap_stackmap;
    ui->listWidget->addItem("new");
    ui->stackedWidget->insertWidget(map_num,current_stack_map);
    total_stack_num.insert(map_num,current_stack_map);
    connect(current_stack_map->ui->directoryButton,SIGNAL(clicked()),this,SLOT(on_currentstack_directory_clicked()));
}

void initializemap::on_okButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Saving initial settings"),".",tr("initial settings files (*.inimap)"));
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("Saving initial settings"),tr("failed to save settings"));
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out<<quint8(map_num);

    foreach(initialmap_stackmap *current_stack_map,total_stack_num){
        if(!current_stack_map->directory_exist)
            return;
        quint32 magic;
        QFile current_map_file(current_stack_map->ui->directoryLabel->text());
        if(!current_map_file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,tr("Saving initial settings"),tr("failed to save settings"));
            return;
        }
        QDataStream in(&current_map_file);
        in.setVersion(QDataStream::Qt_4_8);
        in>>magic;
        if(magic!=map_MagicNum){
            QMessageBox::warning(this,tr("Saving initial settings"),tr("It is not a map file"));
            return;
        }
        out<<current_stack_map->ui->directoryLabel->text();
//        out<<quint8(current_stack_map->ui->paceEdit->text().toInt());
//        if(current_stack_map->position_checked&&
//                (!current_stack_map->ui->rowEdit->text().isEmpty())&&
//                (!current_stack_map->ui->columnEdit->text().isEmpty())){
//            if(pos_checked){
//                QMessageBox::warning(this,tr("Saving initial settings"),tr("there are more than one initializing position\ngrammar mistake!"));
//                return;
//            }
//            out<<bool(1);
	    out<<quint32(current_stack_map->ui->rowEdit->text().toInt())
	      <<quint32(current_stack_map->ui->columnEdit->text().toInt());
//            pos_checked = true;
//        }
//        else
//            out<<bool(0);
    }
    this->close();
}

void initializemap::on_cancelButton_clicked()
{
    int r = QMessageBox::warning(this,tr("Funcatching - Pack page"),
                                 tr("Do you want to quit?"),
                                 QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes==r){
        this->close();
    }
}

void initializemap::closeEvent(QCloseEvent * event)
{
    int r = QMessageBox::warning(this,tr("Funcatching - Pack page"),
                                 tr("Do you want to quit?"),
                                 QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::No==r)
        event->ignore();
    else
        event->accept();
}
