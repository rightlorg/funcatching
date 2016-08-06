#include "server.h"
#include "ui_server.h"
#include <QtNetwork>

Server::Server(QWidget *parent, QTcpSocket *socket, quint32 totalMapNumT) :
    QDialog(parent),
    ui(new Ui::Server)
{
	totalMapNum = totalMapNumT;
	filerecived = 0;
	totalBytes = 0;
	bytesReceived = 0;
	fileNameSize = 0;
	socketConnection = socket;
    ui->setupUi(this);
    connect(socketConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(socketConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));


//    connect(&tcpServer, SIGNAL(newConnection()),
//            this, SLOT(acceptConnection()));
}

Server::~Server()
{
    delete ui;
}

// 开启监听
//void Server::start()
//{
//    if (!tcpServer.listen(QHostAddress::LocalHost, 6666)) {
//        qDebug() << tcpServer.errorString();
//        close();
//        return;
//    }
//    ui->startButton->setEnabled(false);
//    ui->serverStatusLabel->setText(tr("监听"));
//    ui->serverProgressBar->reset();
//}

// 接收连接
//void Server::acceptConnection()
//{
//    tcpServerConnection = tcpServer.nextPendingConnection();
//    connect(tcpServerConnection, SIGNAL(readyRead()),
//            this, SLOT(updateServerProgress()));
//    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(displayError(QAbstractSocket::SocketError)));
//    ui->serverStatusLabel->setText(tr("接受连接"));
//    // 关闭服务器，不再进行监听
//    tcpServer.close();
//}

// 接收文件并更新进度条
void Server::updateServerProgress()
{
    QDataStream in(socketConnection);
    in.setVersion(QDataStream::Qt_4_8);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (bytesReceived <= sizeof(qint64)*2) {
	if((socketConnection->bytesAvailable() >= sizeof(qint64)*2)
                && (fileNameSize == 0)) {
            // 接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
	if((socketConnection->bytesAvailable() >= fileNameSize)
                && (fileNameSize != 0)) {
            // 接收文件名，并建立文件
            in >> fileName;
            ui->serverStatusLabel->setText(tr("接收文件 %1 …")
                                           .arg(fileName));
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) {
                qDebug() << "server: open file error!";
                return;
            }
        } else {
            return;
        }
    }
    // 如果接收的数据小于总数据，那么写入文件
    if (bytesReceived < totalBytes) {
	bytesReceived += socketConnection->bytesAvailable();
	inBlock = socketConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesReceived);

    // 接收数据完成时
    if (bytesReceived == totalBytes) {
//	socketConnection->close();
        localFile->close();
	QDir dir;
	dir.cd("map");
	dir.mkdir("remotemap");
	QFile::copy(localFile->fileName(), "./map/remotemap/" + localFile->fileName());
        ui->serverStatusLabel->setText(tr("接收文件 %1 成功！")
                                       .arg(fileName));
	filerecived++;
	if (filerecived == totalMapNum) {
		emit done();
	}

    }

}

// 显示错误
void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketConnection->errorString();
//    socketConnection->close();
    ui->serverProgressBar->reset();
    ui->serverStatusLabel->setText(tr("服务端就绪"));
}

// 开始监听按钮
//void Server::on_startButton_clicked()
//{
//    start();
//}
