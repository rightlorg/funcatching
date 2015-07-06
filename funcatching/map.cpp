#include <QtGui>
#include "map.h"
mapEditor::mapEditor(QWidget *parent) :
        QWidget(parent),
      ui(new Ui::mapEditor)
{
    ui->setupUi(this);
    menuBar = new QMenuBar;
    fileMenu = new QMenu;
    toolMenu = new QMenu;
    createMenuBar();
}
mapEditor::~mapEditor()
{
    delete ui;
}

void mapEditor::createMenuBar()
{
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(0,0,1000,20);

    fileMenu = menuBar->addMenu(tr("File"));
    actionNew = fileMenu->addAction("New");
    actionOpen = fileMenu->addAction("Open");
    actionSave = fileMenu->addAction("Save");
    actionQuit = fileMenu->addAction("Quit");

    toolMenu = menuBar->addMenu(tr("Tool"));

    connect(actionOpen,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(quitFile()));
}
void mapEditor::openFile()
{
//   QString filename = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.data)"));
}
void mapEditor::saveFile()
{

}

void mapEditor::newFile()
{

}

void mapEditor::quitFile()
{

    this->close();
    delete ui;
}
#if 0
void settings::on_mapButton_clicked()
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
