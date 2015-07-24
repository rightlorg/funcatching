#include "server_start.h"
#include "ui_server_start.h"
#include "server.h"

server_start::server_start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server_start)
{
    ui->setupUi(this);
}

server_start::~server_start()
{
    delete ui;
}

void server_start::on_startorstopButton_clicked()
{
    server Server;
    if(!Server.listen(QHostAddress::Any,2048)){
        return ;
    }
}
