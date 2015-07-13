#include <QtGui>
#include <QDebug>
#include "mapeditor.h"
#include <QDebug>

MapEditor::MapEditor(QWidget *parent) :
        QMainWindow(parent),
      ui(new Ui::MapEditor)
{
    ui->setupUi(this);
    statusImage = new QPixmap;
    createTableWidget(20,20);
    createStatusBar();
    createMenuBar();
    itemstatusLabel = new QLabel;
    itemstatusLabel->setText("VGlass");
    ui->tableWidget->setCurrentCell(0,0);
     statusImage->load("./image/pix2.png");
}

MapEditor::~MapEditor()
{
    delete ui;
}

void MapEditor::createTableWidget(int a,int b)
{
    ui->tableWidget->setRowCount(a);//设置行数为a
    ui->tableWidget->setColumnCount(b);//设置列数为b
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() <<("1"));
    ui->tableWidget->setVerticalHeaderLabels(QStringList()<<("1"));

    for(int row = 0;row<a;++row){
        for(int column = 0;column<b;++column){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            ui->tableWidget->setItem(row,column,item);
        }
    }

    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
}

void MapEditor::createMenuBar()
{
    connect(ui->action_Open,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(ui->action_New,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(ui->action_Save,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(ui->action_Quit,SIGNAL(triggered()),this,SLOT(quitFile()));

    connect(ui->action_About_us,SIGNAL(triggered()),this,SLOT(aboutFile()));
    connect(ui->actionVersion,SIGNAL(triggered()),this,SLOT(ver()));

    connect(ui->actionTool_Dialog,SIGNAL(triggered()),this,SLOT(dockDialog()));
    connect(ui->actionAdd_new_Column,SIGNAL(triggered()),this,SLOT(add_new_column()));
    connect(ui->actionAdd_new_row,SIGNAL(triggered()),this,SLOT(add_new_row()));
    connect(ui->action_View,SIGNAL(triggered()),this,SLOT(viewButton()));
}

void MapEditor::createStatusBar()
{
    statusLabel = new QLabel;
    statusLabel->setText(tr("ready"));
    ui->statusbar->addWidget(statusLabel);
}

bool MapEditor::openFile()
{
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));

   QString filename = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.map)"));
   QFile file(filename);

   if(!file.open(QIODevice::ReadOnly)){
       QMessageBox::warning(this,tr("Map editor"),
                                                        tr("failed to read file %1:\n%2")
                            .arg(file.fileName())
                            .arg(file.errorString()));
               return false;
   }
   QDataStream in(&file);
   in.setVersion(QDataStream::Qt_4_8);

   quint32 magic;
   quint32 temp_row;
   quint32 temp_column;

   in>>magic;

   if(magic!=MagicNum){
       QMessageBox::warning(this,tr("Map editor"),
                            tr("This file is mot a Map file\nPlease rechoose the edited file"));
       return false;
   }

   in>>temp_row;
   in>>temp_column;

   newFile();
   createTableWidget(temp_row,temp_column);
   disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));

   quint16 row;
   quint16 column;
   QString str;

   QApplication::setOverrideCursor(Qt::WaitCursor);
   while(!in.atEnd()){
       in>>row>>column>>str;
       ui->tableWidget->item(row,column)->setText(str);
       qDebug()<<row<<column<<str;
   }
   QApplication::restoreOverrideCursor();

   connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
   return true;
}

void MapEditor::saveFile()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("choose an existing file to open"),".",tr("Map files (*.map)"));
    qDebug()<<filename;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("Saving files"),tr("failed to save file"));
    }
    else
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_8);

        out<<quint32(MagicNum);
        out<<quint32(ui->tableWidget->rowCount());
        out<<quint32(ui->tableWidget->columnCount());

         QApplication::setOverrideCursor(Qt::WaitCursor);
         for(int row = 0;row<(ui->tableWidget->rowCount());++row)
        {
            for(int column = 0;column<(ui->tableWidget->columnCount());++column)
             {
                QString str = ui->tableWidget->item(row,column)->text();
                if(!str.isEmpty())
                {
                     out<<quint16(row)<<quint16(column)<<str;
                     qDebug()<<row<<column<<str;
                }
                else
                    continue;
             }
         }
         QApplication::restoreOverrideCursor();
      }
}

void MapEditor::newFile()
{
    for(int a = 0;a<ui->tableWidget->rowCount();++a){
        for(int b = 0;b<ui->tableWidget->columnCount();++b){
            delete ui->tableWidget->item(a,b);
        }
    }
}

void MapEditor::quitFile()
{
        int r = QMessageBox::warning(this,tr("Map editor"),
                                     tr("Do you want to quit?"),
                                     QMessageBox::Yes|QMessageBox::No);
        if(QMessageBox::Yes==r){
            this->close();
            delete ui;
        }
}

void MapEditor::cell_paint(QTableWidgetItem *item)
{
    item->setText(itemstatusLabel->text());
    //item->setIcon(QIcon(*statusImage));
}

void MapEditor::dockDialog()
{
    if(ui->dockWidget->isHidden()){
        ui->dockWidget->show();
    }else{
        ui->dockWidget->raise();
        ui->dockWidget->activateWindow();
    }
}

void MapEditor::aboutFile()
{
    QMessageBox::about(this,tr("about funcatching"),
                       tr("Copyright &copy; Right and Leo."
                       "<p>funcatching is a game which we made the first time"
                       "<p>provided updating support and server support"));
}


void MapEditor::ver()
{
    QMessageBox::about(this,tr("about funcatching"),
                       tr("funcatching 0.0.0 beta"));
}

#if 0
void MapEditor::on_mapButton_clicked()
{
    //map *mapDialog = new map;
    //delete ui;
    //mapDialog->show();

    filename = QFileDialog::getOpenFileName(this,tr("choose your image"),".",tr("png(*.png)"));
    QSettings imageSet("Carrow Inc.","fun catching");
    imageSet.setValue("imagePath",filename);
    QString filename;
        filename=QFileDialog::getOpenFileName(this,
                                              tr("选择图像"),
                                              ".",
                                              tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
        if(filename.isEmpty())
        {
             return;
        }
        else
        {
            QImage* img=new QImage;

            if(! ( img->load(filename) ) ) //加载图像
            {
                QMessageBox::information(this,
                                         tr("打开图像失败"),
                                         tr("打开图像失败!"));
                delete img;
                return;
            }
            ui->label->setPixmap(QPixmap::fromImage(*img));
        }

}
#endif

void MapEditor::on_VGlass_clicked()
{
    itemstatusLabel->setText(tr("VGlass"));
    statusLabel->setText(tr("vertical glass item choosed"));
}

void MapEditor::on_HGlass_clicked()
{
    itemstatusLabel->setText(tr("HGlass"));
    statusLabel->setText(tr("Horizontal glass item choosed"));
}

void MapEditor::on_VWall_clicked()
{
    itemstatusLabel->setText(tr("VWall"));
    statusLabel->setText(tr("Vertical wall item choosed"));
}

void MapEditor::on_HWall_clicked()
{
    itemstatusLabel->setText(tr("HWall"));
    statusLabel->setText(tr("Horizontal wall item choosed"));
}

void MapEditor::on_VDoor_clicked()
{
    itemstatusLabel->setText(tr("VDoor"));
    statusLabel->setText(tr("Vertical door item choosed"));
}

void MapEditor::on_HDoor_clicked()
{
    itemstatusLabel->setText(tr("HDoor"));
    statusLabel->setText(tr("Horizontal door item choosed"));
}

void MapEditor::on_Floor_clicked()
{
    itemstatusLabel->setText(tr("Floor"));
    statusLabel->setText(tr("Horizontal door item choosed"));
}

void MapEditor::on_nullButton_clicked()
{
    itemstatusLabel->setText(tr(""));
    statusLabel->setText(tr("Clearing item choosed"));
}

void MapEditor::add_new_row()
{
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
    int row=QInputDialog::getInteger(this,tr("Adding new rows"),tr("Please input the number of the rows you want to add"),
                                     statusLabel->text().toInt(),1,100,1);
    for(int i=0;i<row;i++){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        for(int column = 0;column<ui->tableWidget->columnCount();column++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,column,item);
        }
    }
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
}

void MapEditor::add_new_column()
{
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
    int column=QInputDialog::getInteger(this,tr("Adding new rows"),tr("Please input the number of the rows you want to add"),
                                     statusLabel->text().toInt(),1,100,1);
    for(int i=0;i<column;i++){
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        for(int row = 0;row<ui->tableWidget->rowCount();row++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            ui->tableWidget->setItem(row,ui->tableWidget->columnCount()-1,item);
        }
    }
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
}

void MapEditor::viewButton()
{

}

void MapEditor::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::warning(this,tr("Map editor"),
                                 tr("Do you want to quit?"),
                                 QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes==r){
        event->accept();
    }
    else
        event->ignore();
}


