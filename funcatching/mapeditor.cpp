#include <QtGui>
#include <QDebug>
#include <QTableWidgetSelectionRange>
#include "mapeditor.h"
#include "gotocell.h"

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
    statusImage->load(":/new/prefix1/image/pix2.png");

    setWindowIcon(QIcon(":/new/prefix1/image/pix.png"));
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
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
}

void MapEditor::createMenuBar()
{
    connect(ui->action_Open,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(ui->action_Save,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(ui->action_Quit,SIGNAL(triggered()),this,SLOT(quitFile()));

    connect(ui->action_About_us,SIGNAL(triggered()),this,SLOT(aboutFile()));
    connect(ui->actionVersion,SIGNAL(triggered()),this,SLOT(ver()));

    connect(ui->actionTool_Dialog,SIGNAL(triggered()),this,SLOT(dockDialog()));
    connect(ui->actionAdd_new_Column,SIGNAL(triggered()),this,SLOT(add_new_column()));
    connect(ui->actionAdd_new_row,SIGNAL(triggered()),this,SLOT(add_new_row()));
    connect(ui->actionSet_Column_size,SIGNAL(triggered()),this,SLOT(setColumnSize()));
    connect(ui->actionSet_Row_size,SIGNAL(triggered()),this,SLOT(setRowSize()));
    connect(ui->actionGo_to_cell,SIGNAL(triggered()),this,SLOT(gotoCell()));
    connect(ui->action_View,SIGNAL(triggered()),this,SLOT(viewButton()));
    connect(ui->action_Bat,SIGNAL(triggered()),this,SLOT(bat_table()));

    connect(ui->actionClay,SIGNAL(triggered()),this,SLOT(on_Clay_clicked()));
    connect(ui->actionGlass,SIGNAL(triggered()),this,SLOT(on_Glass_clicked()));
    connect(ui->actionWood,SIGNAL(triggered()),this,SLOT(on_Wood_clicked()));

    ui->actionWood->setCheckable(true);
    ui->actionGlass->setCheckable(true);
    ui->actionClay->setCheckable(true);

    ui->actionWood->setChecked(1);
    ui->actionGlass->setChecked(0);
    ui->actionClay->setChecked(0);

    ui->action_Bat->setShortcut(tr("Ctrl+B"));
    ui->action_Open->setShortcut(tr("Ctrl+O"));
    ui->action_Save->setShortcut(QKeySequence::Save);

    ui->actionOpen->setIcon(QIcon(":/new/prefix1/image/open.png"));
}

void MapEditor::createStatusBar()
{
    statusLabel = new QLabel;
    statusLabel->setText(tr("ready"));
    ui->statusbar->addWidget(statusLabel);
}

bool MapEditor::openFile()
{
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
    on_nullButton_clicked();
    for(int a = 0;a<ui->tableWidget->rowCount();++a){
        for(int b = 0;b<ui->tableWidget->columnCount();++b){
            ui->tableWidget->setCurrentCell(a,b);
            bat_table();
        }
    }
    createTableWidget(temp_row,temp_column);

    quint16 row;
    quint16 column;
    QString str;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd()){
        in>>row>>column>>str;
        ui->tableWidget->item(row,column)->setText(str);
    }
    QApplication::restoreOverrideCursor();
    ui->tableWidget->setCurrentCell(0,0);
    return true;
}

void MapEditor::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Saving map"),".",tr("Map files (*.map)"));
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
                out<<quint16(row)<<quint16(column)<<str;
            }
        }
        QApplication::restoreOverrideCursor();
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
    statusImage->load(":/new/prefix1/image/VDoor.png");
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
    itemstatusLabel->setText(tr("WFloor"));
    on_Wood_clicked();
}

void MapEditor::on_nullButton_clicked()
{
    statusImage->load(":/new/prefix1/image/white.png");
    itemstatusLabel->setText(tr(""));
    statusLabel->setText(tr("Clearing item choosed"));
}

void MapEditor::add_new_row()
{
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
    int row=QInputDialog::getInteger(this,tr("Adding new rows"),tr("Please input the number of the rows you want to add"),
                                     statusLabel->text().toInt(),1,300,1);
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
    int column=QInputDialog::getInteger(this,tr("Adding new columns"),tr("Please input the number of the columns you want to add"),
                                        statusLabel->text().toInt(),1,300,1);
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

void MapEditor::on_Wood_clicked()
{
    if(itemstatusLabel->text()=="CFloor"||itemstatusLabel->text()=="GFloor"){
        itemstatusLabel->setText("WFloor");
        statusLabel->setText("Wooden Floor item choosed");
    }
    ui->actionClay->setChecked(0);
    ui->actionGlass->setChecked(0);
    ui->actionWood->setChecked(1);
}

void MapEditor::on_Clay_clicked()
{
    if(itemstatusLabel->text()=="WFloor"||itemstatusLabel->text()=="GFloor"){
        itemstatusLabel->setText("CFloor");
        statusLabel->setText("Clay Floor item choosed");
    }
    ui->actionWood->setChecked(0);
    ui->actionGlass->setChecked(0);
    ui->actionClay->setChecked(1);
}

void MapEditor::on_Glass_clicked()
{
    if(itemstatusLabel->text()=="WFloor"||itemstatusLabel->text()=="CFloor"){
        itemstatusLabel->setText("GFloor");
        statusLabel->setText("Glass Floor item choosed");
    }
    ui->actionWood->setChecked(0);
    ui->actionClay->setChecked(0);
    ui->actionGlass->setChecked(1);
}

void MapEditor::setColumnSize()
{
    int columnsize=QInputDialog::getInteger(this,tr("Setting column size"),tr("Please input the size of column(from 50 to 150)"),
                                            statusLabel->text().toInt(),75,150,50);
    for(int column = 0;column<ui->tableWidget->columnCount();++column)
        ui->tableWidget->setColumnWidth(column,columnsize);
}

void MapEditor::setRowSize()
{
    int rowsize=QInputDialog::getInteger(this,tr("Setting row size"),tr("Please input the size of row(from 20 to 60)"),
                                         statusLabel->text().toInt(),35,60,20);
    for(int row = 0;row<ui->tableWidget->rowCount();++row)
        ui->tableWidget->setRowHeight(row,rowsize);
}

void MapEditor::gotoCell()
{
    GoToCellDialog *gotocell = new GoToCellDialog(this);
    if(gotocell->exec()){
        int column = gotocell->getColumn();
        int row = gotocell->getRow();
        if(column<ui->tableWidget->columnCount() && row<ui->tableWidget->rowCount())
            ui->tableWidget->setCurrentCell(row,column);
        else
            QMessageBox::warning(this,tr("go to cell"),tr("unexpected input"));
    }

}

void MapEditor::bat_table()
{
   QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
   QTableWidgetSelectionRange range = ranges.first();
   for(int row = 0;row<range.rowCount();++row){
       for(int column = 0;column<range.columnCount();++column){
           QTableWidgetItem *item = ui->tableWidget->item(row+range.topRow(),column+range.leftColumn());
           if(itemstatusLabel->text().isEmpty()){
               item->setBackgroundColor(QColor(qRgb(255,255,255)));
               item->setText("");
               ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

           }else{
               label = new QLabel;
               label->setPixmap(*statusImage);
               ui->tableWidget->setCellWidget(item->row(), item->column(), label);
               item->setText(itemstatusLabel->text());
               ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
           }
       }
   }
}

void MapEditor::on_tableWidget_clicked(const QModelIndex &index)
{
    QTableWidgetItem *item = ui->tableWidget->item(index.row(),index.column());
    if(itemstatusLabel->text().isEmpty()){
          item->setBackgroundColor(QColor(qRgb(255,255,255)));
          item->setText("");
    }else{
        label = new QLabel;
        label->setPixmap(*statusImage);
        ui->tableWidget->setCellWidget(item->row(), item->column(), label);
          item->setText(itemstatusLabel->text());
    }
}
