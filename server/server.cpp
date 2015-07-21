#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    label_setup();
    server_setup();
    connect(ui->runButton,SIGNAL(clicked()),this,SLOT(run_order()));
}

server::~server()
{
    delete ui;
}

void server::label_setup()
{
    ui->label->setText("ver:0.0.0bete");
    ui->label_2->setText("Funcatching Server");
    ui->label_3->setText("");
    ui->label_4->setText("");
    ui->label_5->setText("");
    ui->label_6->setText("");
    ui->label_7->setText("");
    ui->label_8->setText("");
    ui->label_9->setText("");
    ui->label_10->setText("");
    ui->label_11->setText("");
    ui->label_12->setText("");
    ui->label_13->setText("");
    ui->label_14->setText("");
    ui->label_15->setText("");
}

void server::server_setup()
{

}

void server::run_order()
{
    QString order = ui->lineEdit->text();
    if(acceptable(order)){

    }
}

bool server::acceptable(QString order)
{
    QStringList orders = order.split(" ");
    return 1;
}
