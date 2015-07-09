#include <QtGui>
#include <QDebug>
#include "mapeditor.h"
#include <QDebug>
MapEditor::MapEditor(QWidget *parent) :
        QMainWindow(parent),
      ui(new Ui::MapEditor)
{
    ui->setupUi(this);
    statusImage = new QImage;
    createTableWidget();
    createStatusBar();
    createMenuBar();
     statusImage->load("./pix2.png");
      //ui->label->setPixmap(QPixmap::fromImage(*statusImage));
}

MapEditor::~MapEditor()
{
    delete ui;

}

void MapEditor::createTableWidget()
{
    ui->tableWidget->setRowCount(999);//设置行数为999
    ui->tableWidget->setColumnCount(999);//设置列数为999
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() <<("1"));
    ui->tableWidget->setVerticalHeaderLabels(QStringList()<<("1"));

    connect(ui->tableWidget,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(cell_paint(QTableWidgetItem*)));
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
}

void MapEditor::createStatusBar()
{
    statusLabel = new QLabel;
    statusLabel->setText(tr("ready"));
    ui->statusbar->addWidget(statusLabel);
}

void MapEditor::openFile()
{
   //QString filename = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.data)"));
}

void MapEditor::saveFile()
{

}

void MapEditor::newFile()
{

}

void MapEditor::quitFile()
{
    this->close();
    delete ui;
}

void MapEditor::cell_paint(QTableWidgetItem *item)
{
    qDebug("clicked");
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
