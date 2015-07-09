#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "readypage.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        statusLabel = new QLabel;
//        statusImage = new QImage;
        createStatusBar();
//        QString filename;
//            filename=QFileDialog::getOpenFileName(this,
//                                                  tr("选择图像"),
//                                                  ".",
//                                                  tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
//            if(filename.isEmpty())
//            {
//                 return;
//            }
//            else
//            {
//       QImage* img=new QImage;

//               if(! ( img->load(filename) ) ) //加载图像
//               {
//                   QMessageBox::information(this,
//                                            tr("打开图像失败"),
//                                            tr("打开图像失败!"));
//                   delete img;
//                   return;
//               }
//               ui->label->setPixmap(QPixmap::fromImage(*img));
//            }
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::on_startGame_clicked()
{
    ui->startWidget->hide();
    ReadyPage *readyPage = new ReadyPage(this);
    ui->gridLayout_main->addWidget(readyPage);

    statusLabel->setText("Getting ready");
}

void MainWindow::on_settings_clicked()
{
   // ui->centralWidget->hide();
    settings *SettingsDialog = new settings;
    statusLabel->setText("general settings");
    SettingsDialog->exec();
}

void MainWindow::createStatusBar()
{
    statusLabel->setText("game loading");
    statusBar()->addWidget(statusLabel);
}

void MainWindow::showStartPage()
{
    ui->startWidget->show();
}
