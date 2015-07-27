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
    table_view_size = 2;
    createTableWidget(20,20);
    createStatusBar();
    createMenuBar();
    itemstatusString = "VGlass";
    ui->tableWidget->setCurrentCell(0,0);

    setWindowIcon(QIcon(":/image/pix.png"));
}

MapEditor::~MapEditor()
{
    ui->tableWidget->clearContents();
    qDebug()<<"asdfas";
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
    ui->tableWidget->setAutoFillBackground(true);
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
    connect(ui->actionGo_to_cell,SIGNAL(triggered()),this,SLOT(gotoCell()));
    connect(ui->action_Bat,SIGNAL(triggered()),this,SLOT(bat_table()));
    connect(ui->actionSize,SIGNAL(triggered()),this,SLOT(adjust_table_size()));
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

    ui->actionGo_to_cell->setIcon(QIcon(":/image/gotocell.png"));
    ui->action_Open->setIcon(QIcon(":/image/open.png"));
    ui->action_Save->setIcon(QIcon(":/image/save.png"));
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
    ui->tableWidget->clearContents();

    createTableWidget(temp_row,temp_column);

    quint16 row;
    quint16 column;
    QString str;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd()){
        in>>row>>column>>str;
        initialize_item(row,column,str);
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
    itemstatusString = "VGlass";
    statusLabel->setText(tr("vertical glass item choosed"));
}

void MapEditor::on_HGlass_clicked()
{
    itemstatusString = "HGlass";
    statusLabel->setText(tr("Horizontal glass item choosed"));
}

void MapEditor::on_VWall_clicked()
{
    itemstatusString = "VWall";
    statusLabel->setText(tr("Vertical wall item choosed"));
}

void MapEditor::on_grass_clicked()
{
    statusImage->load(":/image/grass.png");
    itemstatusString = "grass";
    statusLabel->setText(tr("grass item choosed"));
}

void MapEditor::on_HWall_clicked()
{
    itemstatusString = "HWall";
    statusLabel->setText(tr("Horizontal wall item choosed"));
}

void MapEditor::on_VDoor_clicked()
{
    statusImage->load(":/image/VDoor.png");
    itemstatusString = "VDoor";
    statusLabel->setText(tr("Vertical door item choosed"));
}

void MapEditor::on_HDoor_clicked()
{
    itemstatusString = "HDoor";
    statusLabel->setText(tr("Horizontal door item choosed"));
}

void MapEditor::on_Floor_clicked()
{
    statusImage->load(":/tex/3.png");
    *statusImage = statusImage->scaled(45, 40);
    itemstatusString = "WFloor";
    on_Wood_clicked();
}

void MapEditor::on_nullButton_clicked()
{
    statusImage->load(":/image/white.png");
    itemstatusString = "";
    statusLabel->setText(tr("Clearing item choosed"));
}

void MapEditor::add_new_row()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
    int row=QInputDialog::getInteger(this,tr("Adding new rows"),tr("Please input the number of the rows you want to add"),
                                     statusLabel->text().toInt(),1,300,1);
    ui->progressBar->setMaximum(row);

    for(int i=0;i<row;i++){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        for(int column = 0;column<ui->tableWidget->columnCount();column++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,column,item);
            ui->progressBar->setValue(row);
        }
    }
    QApplication::restoreOverrideCursor();
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
}

void MapEditor::add_new_column()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
    int column=QInputDialog::getInteger(this,tr("Adding new columns"),tr("Please input the number of the columns you want to add"),
                                        statusLabel->text().toInt(),1,300,1);
    ui->progressBar->setMaximum(column);
    for(int i=0;i<column;i++){
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        for(int row = 0;row<ui->tableWidget->rowCount();row++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            ui->tableWidget->setItem(row,ui->tableWidget->columnCount()-1,item);
            ui->progressBar->setValue(column);
        }
    }
    QApplication::restoreOverrideCursor();
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
}

//void MapEditor::closeEvent(QCloseEvent *event)
//{
//        int r = QMessageBox::warning(this,tr("Map editor"),
//                                     tr("Do you want to quit?"),
//                                     QMessageBox::Yes|QMessageBox::No);
//        if(QMessageBox::Yes==r){
//            event->accept();
//        }
//        else
//            event->ignore();
//}

void MapEditor::on_Wood_clicked()
{
    if(itemstatusString=="CFloor"||itemstatusString=="GFloor"){
        itemstatusString = "WFloor";
        statusLabel->setText("Wooden Floor item choosed");
    }
    ui->actionClay->setChecked(0);
    ui->actionGlass->setChecked(0);
    ui->actionWood->setChecked(1);
}

void MapEditor::on_Clay_clicked()
{
    if(itemstatusString=="WFloor"||itemstatusString=="GFloor"){
        itemstatusString = "CFloor";
        statusLabel->setText("Clay Floor item choosed");
    }
    ui->actionWood->setChecked(0);
    ui->actionGlass->setChecked(0);
    ui->actionClay->setChecked(1);
}

void MapEditor::on_Glass_clicked()
{
    if(itemstatusString=="WFloor"||itemstatusString=="CFloor"){
        itemstatusString = "GFloor";
        statusLabel->setText("Glass Floor item choosed");
    }
    ui->actionWood->setChecked(0);
    ui->actionClay->setChecked(0);
    ui->actionGlass->setChecked(1);
}

void MapEditor::adjust_table_size()
{
    qDebug()<<"adfasd";
    adjust_size *ad = new adjust_size(this,table_view_size);
    while(ad->exec())
    {
        table_view_size = ad->table_size;
        for(int row = 0;row<ui->tableWidget->rowCount();++row)
            ui->tableWidget->setRowHeight(row,table_view_size*15);
        for(int column = 0;column<ui->tableWidget->columnCount();++column)
            ui->tableWidget->setColumnWidth(column,table_view_size*15);
//        foreach(QWidget *win,ui->tableWidget){
//            win->sets
//        }
    }
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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    ui->tableWidget->setIconSize(QSize(ui->tableWidget->rowHeight(0),ui->tableWidget->columnWidth(0)));
    ui->progressBar->setMaximum(range.rowCount()*range.columnCount());
    for(int row = 0;row<range.rowCount();++row){
        for(int column = 0;column<range.columnCount();++column){
            QTableWidgetItem *item = ui->tableWidget->item(row+range.topRow(),column+range.leftColumn());
            {
                label = new QLabel;
                label->setPixmap(*statusImage);
                ui->tableWidget->setCellWidget(item->row(), item->column(), label);
                item->setText(itemstatusString);

                ui->progressBar->setValue(row*range.rowCount()+column);
            }
        }
    }
    QApplication::restoreOverrideCursor();
    ui->progressBar->setValue(range.rowCount()*range.columnCount());
}

void MapEditor::on_tableWidget_clicked(const QModelIndex &index)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    QTableWidgetItem *item = ui->tableWidget->item(index.row(),index.column());
    {
        label = new QLabel;
        label->setPixmap(*statusImage);
        ui->tableWidget->setCellWidget(item->row(), item->column(), label);
        item->setText(itemstatusString);
    }
    QApplication::restoreOverrideCursor();
}

void MapEditor::initialize_item(int row,int column,QString status)
{
    label = new QLabel;
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    ui->tableWidget->setIconSize(QSize(ui->tableWidget->rowHeight(0),ui->tableWidget->columnWidth(0)));
    if(status.toAscii()=="grass"){
        label->setPixmap(QPixmap(":/image/grass"));
        ui->tableWidget->setCellWidget(item->row(), item->column(), label);
        item->setText("grass");
    }else if(status.toAscii()=="VDoor"){
        label->setPixmap(QPixmap(":/image/grass"));
        ui->tableWidget->setCellWidget(item->row(), item->column(), label);
        item->setText("grass");
    }
}
