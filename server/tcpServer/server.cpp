#include "server.h"
#include "ui_server.h"
#include <QtNetwork>
#include <QImageReader>



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
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(2049, QUdpSocket::ShareAddress);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(startUdpTimer()));

	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(readingController()));
	//    connect(this, SIGNAL(currentDataExchangeDone()), SLOT(nextConnection()));
	totalClientNum = 0;
	blockSize = 0;
	currentClient = NULL;
	readyToReadNext = true;
	mapSent = 0;sendMapClient = NULL;



}

Server::~Server()
{
	delete ui;
}

void Server::startUdpTimer()
{
	disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(startUdpTimer()));
	connect(&timer, SIGNAL(timeout()), SLOT(timerUpdate()));
	timer.start(1000 / GAME_TICK);


}

bool Server::loadMap()
{
	QDir dir("./");
	dir.setSorting(QDir::Time);
	if (!dir.cd("map"))
	{
		dir.mkdir("map");
		QMessageBox::warning(NULL, tr("警告"), tr("Please put the map in ./map/"),
				     QMessageBox::Abort);
		return false;
	}
	//	map = new Map(this, dir.path());
	//	if (!map->readInitMapFile()) {
	//		return false;
	//	}
	//	if(!map->loadMap()) {
	//		return false;
	//	}
	map = new Map(this, dir.path());
	if (!map->readInitMapFile()) {
		return false;
	}
	if(!map->loadMap()) {
		return false;
	}
	spawnPoint = map->getSpawnPoint(0);
	mapPath = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < mapPath.size(); ++i) {
		mapPath[i] = dir.absoluteFilePath(mapPath[i]);
	}
	qDebug() << mapPath;
	
	return  true;
}

void Server::sendAllPlayer()
{
	QTcpSocket *tempSocket;

	for (int i = 0; i < playerList.size(); ++i) {
		tempSocket = playerList[i].socket;
		QByteArray block;
		QDataStream out(&block,QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_8);

		out << qint64(0) ;
		 out << qint64(i) ;//myplayer Id
		out<<qint64(playerList.size());
		for (int j = 0; j < playerList.size(); ++j) {
			if (j == i) {
				continue;
			}
			QBuffer imageBuffer;
			imageBuffer.open(QIODevice::ReadWrite);
			playerList[j].playerImage.save(&imageBuffer, "PNG");

			out << qint64(j);	// playerId
			out << playerList[j].playerName;
			out << qint64(imageBuffer.data().size());
			block.append(imageBuffer.data());
//			out.device()->seek(block.size());

		}
		out.device()->seek(0);
		out<<qint64(block.size()-sizeof(qint64));

		tempSocket->write(block);
		disconnect(currentClient, SIGNAL(readyRead()), this, SLOT(exchangeData()));
	}
}

void Server::writeDatagrame()
{
	for (int j = 0; j < playerList.size(); ++j) {
		QByteArray datagram;
		QDataStream out(&datagram,QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_8);
		for (int i = 0; i < playerList.size(); ++i) {
			out << qint64(i);
			out << playerList[i].pos;

		}
		udpSocket->writeDatagram(datagram.data(), datagram.size(),
				      playerList[j].address, 2049);
	}


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
	mapSent = 0;
	connect(currentClient, SIGNAL(readyRead()), this, SLOT(exchangeData()));

}

void Server::timerUpdate()
{
	writeDatagrame();
}

void Server::sendTotalMapNum()
{
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);

	qDebug() << mapPath.size();
	out.setVersion(QDataStream::Qt_4_8);
	out << qint32(0) << qint32(mapPath.size());
	out.device()->seek(0);
	out<<qint32(block.size()-sizeof(qint32));
	currentClient->write(block);
	emit sendTotalMapNumDone();

}

void Server::sendNextMap()
{
	disconnect(currentClient, SIGNAL(bytesWritten(qint64)), this, SLOT(sendNextMap()));
	if (sendMapClient != NULL) {
		disconnect(sendMapClient, SIGNAL(done()), this, SLOT(sendNextMap()));
		delete sendMapClient;
		sendMapClient = NULL;
	}
	if (mapSent == mapPath.size()) {
		nextConnection();
		return;
	}

	sendMapClient = new Client(this, currentClient, mapPath[mapSent]);
	mapSent++;

	connect(sendMapClient, SIGNAL(done()), this, SLOT(sendNextMap()));
	if (sendMapClient == NULL) {
		QMessageBox::warning(NULL, tr("警告"),
				     tr("Faild to send map, Please restart the server"),
				     QMessageBox::Abort);
	}
	sendMapClient->startTransfer();

}

void Server::sendMessage()
{
}

void Server::exchangeData()
{
	{
		//getting player's imformation
		QDataStream in(currentClient);
		// 设置数据流版本，这里要和服务器端相同
		in.setVersion(QDataStream::Qt_4_8);
		// 如果是刚开始接收数据`
		if (blockSize == 0) {
			//判断接收的数据是否大于两字节，也就是文件的大小信息所占的空间
			//如果是则保存到blockSize变量中，否则直接返回，继续接收数据
			if(currentClient->bytesAvailable() < (int)sizeof(qint32)) return;
			in >> blockSize;
		}
		// 如果没有得到全部的数据，则返回，继续接收数据
		if(currentClient->bytesAvailable() < blockSize) return;
		// 将接收到的数据存放到变量中
		QString playername;

		qint64 imageBufferSize;
		in >> playername >> imageBufferSize;
		QByteArray a =  currentClient->read(imageBufferSize);
		QBuffer buffer(&a);
		    buffer.open(QIODevice::ReadOnly);
		    QImageReader reader(&buffer,"PNG");
		     QImage playerimage = reader.read();
		     if (playerimage.isNull()) {
			     qDebug() << "fail to read headimage";
		     }
		Player newplayer;
		newplayer.playerName = playername;
		newplayer.playerImage = playerimage;
		newplayer.socket = currentClient;
		newplayer.address = currentClient->peerAddress();
		newplayer.pos.setX(spawnPoint.rx() * 32 + 3);
		newplayer.pos.setY(spawnPoint.ry() * 32 + 3);



		playerList.append(newplayer);
	}
	connect(currentClient, SIGNAL(bytesWritten(qint64)), this, SLOT(sendNextMap()));
	sendTotalMapNum();

	//map
	//	map->sendMap(currentClient);

	//	nextConnection();

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

void Server::processPendingDatagram()
{
	// 拥有等待的数据报
	while(udpSocket->hasPendingDatagrams())
	{
	    QByteArray datagram;
	    qint64 playerId;
	    QPointF playerPos;
	    // 让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
	    datagram.resize(udpSocket->pendingDatagramSize());
	    // 接收数据报，将其存放到datagram中
	    udpSocket->readDatagram(datagram.data(), datagram.size());
	    QDataStream in(&datagram, QIODevice::ReadOnly);
	    in >> playerId >> playerPos;
	    playerList[playerId].pos = playerPos;
	}

}


void Server::on_pushButton_clicked()
{
	sendAllPlayer();
}
