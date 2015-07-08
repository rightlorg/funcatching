#include <QtGui>
#include <QDebug>
#include "mapeditor.h"
MapEditor::MapEditor(QWidget *parent) :
        QMainWindow(parent),
      ui(new Ui::MapEditor)
{
    ui->setupUi(this);
   ui->tableWidget->setRowCount(999);//设置行数为999
   ui->tableWidget->setColumnCount(999);//设置列数为999
   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//编码设置为utf8
   ui->tableWidget->setHorizontalHeaderLabels(QStringList() <<("1"));
   ui->tableWidget->setVerticalHeaderLabels(QStringList()<<("1"));
}
MapEditor::~MapEditor()
{
    delete ui;
}

void MapEditor::createMenuBar()
{
    menubar = new QMenuBar(this);
    menubar->setGeometry(0,0,1000,20);
    fileMenu = menubar->addMenu(tr("File"));
    actionNew = fileMenu->addAction("New");
    actionOpen = fileMenu->addAction("Open");
    actionSave = fileMenu->addAction("Save");
    actionQuit = fileMenu->addAction("Quit");


    toolMenu = menubar->addMenu(tr("Tool"));

    connect(actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(quitFile()));
}

void MapEditor::createStatusBar()
{
    statusLabel->setText("game loading");
    statusBar()->addWidget(statusLabel);
}

void MapEditor::openFile()
{
//   QString filename = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.data)"));
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


void MapEditor::on_treeWidget_clicked(const QModelIndex &index)
{
    qDebug("a");
}
void MapEditor::on_treeWidget_activated(const QModelIndex &index)
{
    ;//input something here in the future
}
