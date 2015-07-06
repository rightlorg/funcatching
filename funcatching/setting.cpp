#include <QtGui>
#include "settings.h"
#include "map.h"
settings::settings(QWidget *parent) :
        QDialog(parent),
      ui(new Ui::settings)
{
        ui->setupUi(this);
}

settings::~settings()
{
    delete ui;
}

void settings::on_backButton_clicked()
{
    this->close();
}

void settings::on_mapButton_clicked()
{
    mapEditor *map= new mapEditor;
    this->close();
    map->show();
#if 0
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
#endif
}
