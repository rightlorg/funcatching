#include "server.h"
#include "ui_server.h"
#include <QtNetwork>

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    if (!loadMap()) {
	    QMessageBox::warning(NULL, tr("警告"), tr("Please restart the server"),
	                                                          QMessageBox::Abort);
    }
    tcpServer = new QTcpServer(this);
    // 使用了IPv4的本地主机地址，等价于QHostAddress("127.0.0.1")
    if (!tcpServer->listen(QHostAddress::LocalHost, 2048)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(readingController()));
        totalClientNum = 0;
    blockSize = 0;
    currentClient = NULL;
    readyToReadNext = true;



}

Server::~Server()
{
	delete ui;
}

bool Server::loadMap()
{
	QDir dir;
	dir.setSorting(QDir::Time);
	if (!dir.cd("map"))
	{
		dir.mkdir("map");
		QMessageBox::warning(NULL, tr("警告"), tr("Please put the map in ./map/"),
		                                                      QMessageBox::Abort);
		return false;
	}
	map = new Map(this, dir.absoluteFilePath(dir.path()));
	if (!map->readInitMapFile()) {
		return false;
	}
	if(!map->loadMap()) {
		return false;
	}
	mapPath = dir.entryList();
	qDebug() << mapPath;
	

}

void Server::nextConnection()
{
	if (currentClient != NULL) {
		disconnect(currentClient, SIGNAL(readyRead()), this, SLOT(exchangeData()));
	}
	currentClient = NULL;
	currentClient = tcpServer->nextPendingConnection();
	if (currentClient == NULL) {
		readyToReadNext = true;
		return;
	}
	blockSize = 0;
	connect(currentClient, SIGNAL(readyRead()), this, SLOT(exchangeData()));

}

void Server::sendMessage()
{
	QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

    // 用于暂存我们要发送的数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // 设置数据流的版本，客户端和服务器端使用的版本要相同
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << tr("hello TCP!!!");
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    // 获取已经建立的连接的套接字
    clientConnection->write(block);
    // 发送数据成功后，显示提示
    ui->label->setText("send message successful!!!");
}

void Server::exchangeData()
{
	//getting player's imformation
	QDataStream in(currentClient);
	// 设置数据流版本，这里要和服务器端相同
	in.setVersion(QDataStream::Qt_4_8);
	// 如果是刚开始接收数据`
	if (blockSize == 0) {
		//判断接收的数据是否大于两字节，也就是文件的大小信息所占的空间
		//如果是则保存到blockSize变量中，否则直接返回，继续接收数据
		if(currentClient->bytesAvailable() < (int)sizeof(quint32)) return;
		in >> blockSize;
	}
	// 如果没有得到全部的数据，则返回，继续接收数据
	if(currentClient->bytesAvailable() < blockSize) return;
	// 将接收到的数据存放到变量中
	QString playername;
	QByteArray a;
	QImage playerimage;
	in >> playername >> a;
	playerimage.loadFromData(a);
	Player newplayer;
	newplayer.playerName = playername;
	newplayer.playerImage = playerimage;
	newplayer.socket = currentClient;
	playerList.append(newplayer);

	//map
	map->sendMap(currentClient);

	nextConnection();

}

void Server::readingController()
{
	ui->label->setText("connect successfully");
	if (readyToReadNext) {
		readyToReadNext = false;
		nextConnection();
	}
}

void Server::addTotalClientNum()
{
	totalClientNum++;
}

