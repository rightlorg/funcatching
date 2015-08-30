#include <QtGui>
#include <QDebug>
#include <QTableWidgetSelectionRange>
#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QIcon>
#include <QHash>
#include "mapeditor.h"
#include "gotocell.h"
#include "blocklist.h"

BlockList blocklist;

MapEditor::MapEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapEditor)
{
    ui->setupUi(this);
    statusImage = new QPixmap;
    __block = -1;
    selection = 0;
    dockbuttonList.append(NULL);
    table_view_size = 2;
    createTableWidget(20,20);
    createStatusBar();
    createMenuBar();
    blockStatus = 0; //no
    itemstatusString = "VGlass";
    ui->tableWidget->setCurrentCell(0,0);
    ui->tableWidget->setIconSize(QSize(32, 32));
    setWindowIcon(QIcon(":/image/pix.png"));
    initdockButtons();
}

MapEditor::~MapEditor()
{
    ui->tableWidget->clearContents();
    delete ui;
}

void MapEditor::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::warning(this,tr("Funcatching"),
                                 tr("Do you want to quit?"),
                                 QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes==r){
        ui->tableWidget->clearContents();
        delete ui;
        event->accept();
    }else
        event->ignore();
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
    connect(ui->actionSmall,SIGNAL(triggered()),this,SLOT(set_small_size()));
    connect(ui->actionMidium,SIGNAL(triggered()),this,SLOT(set_midium_size()));

    connect(ui->actionRight_Wall, SIGNAL(triggered(bool)), this, SLOT(on_rightwall_clicked()));
    connect(ui->actionLeft_Wall, SIGNAL(triggered(bool)), this, SLOT(on_leftwall_clicked()));
    connect(ui->actionTop_Wall,SIGNAL(triggered()),this,SLOT(on_topwall_clicked()));
    connect(ui->actionButtom_Wall,SIGNAL(triggered()),this,SLOT(on_buttomwall_clicked()));

    connect(ui->actionTop_Left,SIGNAL(triggered()),this,SLOT(on_top_left_clicked()));
    connect(ui->actionTop_Right,SIGNAL(triggered()),this,SLOT(on_top_right_clicked()));
    connect(ui->actionButtom_Left,SIGNAL(triggered()),this,SLOT(on_buttom_left_clicked()));
    connect(ui->actionButtom_Right,SIGNAL(triggered()),this,SLOT(on_buttom_right_clicked()));

    connect(ui->action_All,SIGNAL(triggered()),this,SLOT(on_action_all_clicked()));
    connect(ui->actionNO, SIGNAL(triggered(bool)), this, SLOT(onactionNO_clicded()));

    ui->actionWood->setCheckable(true);
    ui->actionGlass->setCheckable(true);
    ui->actionClay->setCheckable(true);
    ui->actionTop_Wall->setCheckable(true);
    ui->actionButtom_Wall->setCheckable(true);
    ui->actionLeft_Wall->setCheckable(true);
    ui->actionRight_Wall->setCheckable(true);
    ui->action_All->setCheckable(true);

    ui->actionTop_Left->setCheckable(true);
    ui->actionTop_Right->setCheckable(true);
    ui->actionButtom_Left->setCheckable(true);
    ui->actionButtom_Right->setCheckable(true);

    ui->actionWood->setChecked(true);
    ui->actionGlass->setChecked(false);
    ui->actionClay->setChecked(false);

    clear_block_status_menu();

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

    if(magic!=map_MagicNum){
        QMessageBox::warning(this,tr("Map editor"),
                             tr("This file is mot a Map file\nPlease rechoose the edited file"));
        return false;
    }

    in>>temp_row;
    in>>temp_column;
    ui->tableWidget->clearContents();

    createTableWidget(temp_row,temp_column);
    in >> temp_row;
    in >> temp_column;
    quint16 row;
    quint16 column;
    quint8 id, status;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    while(!in.atEnd()){
        in>>row>>column>>id>>status;
        initialize_item(row,column,id,status);
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

        out<<quint32(map_MagicNum);
        out<<quint32(ui->tableWidget->rowCount());
        out<<quint32(ui->tableWidget->columnCount());

        quint8 id, status;
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        for(int row = 0;row<(ui->tableWidget->rowCount());++row)
        {
            for(int column = 0;column<(ui->tableWidget->columnCount());++column)
            {
                id = ui->tableWidget->item(row,column)->data(88).toUInt();
                status = ui->tableWidget->item(row, column)->data(66).toUInt();
                out<<quint32(row)<<quint32(column)<<id<<status;
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
                       tr("funcatching 0.0.1 beta"));
}

void MapEditor::on_nullButton_clicked()
{
    selection = 0;
    statusLabel->setText(tr("NULL"));
    statusImage->load(":/image/white.png");
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

void MapEditor::onactionNO_clicded()
{
    clear_block_status_menu();
    __block = -1;
    statusImage->load(":/tex/" + QString::number(selection) + ".png");
}

void MapEditor::ondockbuttonClicked()
{
    if(QPushButton* senderButton = dynamic_cast<QPushButton*>(sender())){
        for (int i = 0; i < dockbuttonList.size(); ++i) {
            if (dockbuttonList[i] == senderButton) {
                selection = i;
                if (blockStatus == 1 && selection != 0)
                    statusImage->load(":/tex/" + QString::number(selection) + "-8.png");
                else{
                    __block=0;
                    update_block_status();
                    statusImage->load(":/tex/" + QString::number(i) + ".png");
                    statusLabel->setText(blocklist.blocklist.key(i));
                }
            }
        }
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
                item->setIcon(QIcon(*statusImage));
                item->setData(88, selection);
                item->setData(66, blockStatus);
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
        item->setIcon(QIcon(*statusImage));
        item->setData(88, selection);
        item->setData(66, blockStatus);
        item->setTextAlignment(Qt::AlignLeft);
    }
    QApplication::restoreOverrideCursor();
}

void MapEditor::initialize_item(int row,int column,int id,int status)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    item->setData(88, id);
    item->setData(66, status);
}

void MapEditor::initdockButtons()
{
    for (quint8 i = 1; i < blocklist.blocklist.size(); ++i) {
        QPushButton *newButton = new QPushButton(ui->dockWidget);
        newButton->setMaximumSize(36, 36);
        newButton->setMinimumSize(36, 36);
        newButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        QString picPath = ":/tex/" + QString::number(i) + ".png";

        newButton->setIcon(QIcon(picPath));
        newButton->setIconSize(QSize(32, 32));
        ui->verticalLayout->addWidget(newButton);
        dockbuttonList.append(newButton);
        connect(newButton, SIGNAL(clicked()), this, SLOT(ondockbuttonClicked()));
    }
}

void MapEditor::add_one_label(int row, int column, QLabel* newlabel)
{
    QMap<int, QLabel*> map;
    map.insert(column,newlabel);
    storing_all_label.insert(row, map);
}


void MapEditor::on_noButton_clicked()
{
    blockStatus = 0;
    if (selection != 0)
        statusImage->load(":/tex/" + QString::number(selection) + ".png");
    clear_block_status_menu();
    ui->actionNO->setChecked(true);
}

void MapEditor::on_wallButton_clicked()
{
    blockStatus = 1;
    if (selection != 0)
        statusImage->load(":/tex/" + QString::number(selection) + "-8.png");
}

void MapEditor::on_topwall_clicked()
{
    if(ui->actionTop_Wall->isChecked()){
        if(ui->actionRight_Wall->isChecked()){
            __block=5;
            statusImage->load(":/tex/" + QString::number(selection) + "-5.png");
            clear_block_status_menu();
            ui->actionTop_Wall->setChecked(true);
            ui->actionRight_Wall->setChecked(true);
        }else if(ui->actionLeft_Wall->isChecked()){
            __block=6;
            statusImage->load(":/tex/" + QString::number(selection) + "-6.png");
            clear_block_status_menu();
            ui->actionTop_Wall->setChecked(true);
            ui->actionLeft_Wall->setChecked(true);
        }else{
            __block=3;
            statusImage->load(":/tex/" + QString::number(selection) + "-3.png");
            clear_block_status_menu();
            ui->actionTop_Wall->setChecked(true);
        }
    }
}

void MapEditor::on_buttomwall_clicked()
{
    if(ui->actionButtom_Wall->isChecked()){
        if(ui->actionRight_Wall->isChecked()){
            __block=7;
            statusImage->load(":/tex/" + QString::number(selection) + "-7.png");
            clear_block_status_menu();
            ui->actionButtom_Wall->setChecked(true);
            ui->actionRight_Wall->setChecked(true);
        }else if(ui->actionLeft_Wall->isChecked()){
            __block=4;
            statusImage->load(":/tex/" + QString::number(selection) + "-4.png");
            clear_block_status_menu();
            ui->actionButtom_Wall->setChecked(true);
            ui->actionLeft_Wall->setChecked(true);
        }else{
            __block=2;
            statusImage->load(":/tex/" + QString::number(selection) + "-2.png");
            clear_block_status_menu();
            ui->actionButtom_Wall->setChecked(true);
        }
    }
}

void MapEditor::on_leftwall_clicked()
{
    if(ui->actionLeft_Wall->isChecked()){
        if(ui->actionTop_Wall->isChecked()){
            __block=6;
            statusImage->load(":/tex/" + QString::number(selection) + "-6.png");
            clear_block_status_menu();
            ui->actionTop_Wall->setChecked(true);
            ui->actionLeft_Wall->setChecked(true);
        }else if(ui->actionButtom_Wall->isChecked()){
            __block=4;
            statusImage->load(":/tex/" + QString::number(selection) + "-4.png");
            clear_block_status_menu();
            ui->actionButtom_Wall->setChecked(true);
            ui->actionLeft_Wall->setChecked(true);
        }else{
            __block=1;
            statusImage->load(":/tex/" + QString::number(selection) + "-1.png");
            clear_block_status_menu();
            ui->actionLeft_Wall->setChecked(true);
        }
    }
}

void MapEditor::on_rightwall_clicked()
{
    if(ui->actionRight_Wall->isChecked()){
        if(ui->actionTop_Wall->isChecked()){
            __block=5;
            statusImage->load(":/tex/" + QString::number(selection) + "-5.png");
            clear_block_status_menu();
            ui->actionTop_Wall->setChecked(true);
            ui->actionRight_Wall->setChecked(true);
        }else if(ui->actionButtom_Wall->isChecked()){
            __block=7;
            statusImage->load(":/tex/" + QString::number(selection) + "-7.png");
            clear_block_status_menu();
            ui->actionButtom_Wall->setChecked(true);
            ui->actionRight_Wall->setChecked(true);
        }else{
            __block=0;
            statusImage->load(":/tex/" + QString::number(selection) + "-0.png");
            clear_block_status_menu();
            ui->actionRight_Wall->setChecked(true);
        }
    }
}

void MapEditor::on_top_left_clicked()
{

}

void MapEditor::on_top_right_clicked()
{

}

void MapEditor::on_buttom_left_clicked()
{

}

void MapEditor::on_buttom_right_clicked()
{

}

void MapEditor::on_action_all_clicked()
{
    clear_block_status_menu();
    ui->action_All->setChecked(true);
    __block=8;
    statusImage->load(":/tex/" + QString::number(selection) + "-8.png");
}

void MapEditor::update_block_status()
{
    if(__block!=-1)
        statusImage->load(":/tex/" + QString::number(selection) + "-" + QString::number(__block) + ".png");
}

void MapEditor::clear_block_status_menu()
{
    ui->actionTop_Wall->setChecked(false);
    ui->actionButtom_Wall->setChecked(false);
    ui->actionLeft_Wall->setChecked(false);
    ui->actionRight_Wall->setChecked(false);

    ui->actionTop_Left->setChecked(false);
    ui->actionTop_Right->setChecked(false);
    ui->actionButtom_Left->setChecked(false);
    ui->actionButtom_Right->setChecked(false);

    ui->actionNO->setChecked(false);
}

void MapEditor::set_small_size()
{
    for(int i = 0;i<ui->tableWidget->rowCount();++i)
        ui->tableWidget->setRowHeight(i,20);
    for(int i = 0;i<ui->tableWidget->columnCount();++i)
        ui->tableWidget->setColumnWidth(i,20);
    ui->tableWidget->setIconSize(QSize(16,16));
}

void MapEditor::set_midium_size()
{
    for(int i = 0;i<ui->tableWidget->rowCount();++i)
        ui->tableWidget->setRowHeight(i,36);
    for(int i = 0;i<ui->tableWidget->columnCount();++i)
        ui->tableWidget->setColumnWidth(i,36);
    ui->tableWidget->setIconSize(QSize(32,32));
}
