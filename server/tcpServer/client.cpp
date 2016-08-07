#include "client.h"
#include "ui_client.h"

#include <QtNetwork>
#include <QFileDialog>

Client::Client(QWidget *parent, QTcpSocket *socket, QString filePath) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    if (filePath.isEmpty()) {
	    delete this;
    }

    payloadSize = 64*1024; // 64KB
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = socket;
    fileName = filePath;
    bytesWritten = 0;
    // 当连接服务器成功时，发出connected()信号，开始传送文件

    connect(tcpClient, SIGNAL(bytesWritten(qint64)),
            this, SLOT(updateClientProgress(qint64)));

    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

}

Client::~Client()
{
    delete ui;
}

// 打开文件

// 连接到服务器

// 传输文件大小等信息
void Client::startTransfer()
{
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly)) {
        qDebug() << "client: open file error!";
        return;
    }
    // 获取文件大小
    totalBytes = localFile->size();

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    // 保留总大小信息空间、文件名大小信息空间，然后输入文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;
    // 这里的总大小是总大小信息、文件名大小信息、文件名和实际文件大小的总和
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
    // 发送完文件头结构后剩余数据的大小
//    qDebug() << outBlock.size();
//    bytesToWrite = totalBytes - tcpClient->write(outBlock);
	tcpClient->write(outBlock);
    outBlock.resize(0);
}

// 发送数据，并更新进度条
void Client::updateClientProgress(qint64 numBytes)
{
	outBlock = localFile->read(localFile->size());
	tcpClient->write(outBlock);
	outBlock.resize(0);
	localFile->close();
	emit done();

//    // 已经发送数据的大小
//    bytesWritten += (int)numBytes;
//    // 如果已经发送了数据
//    if (bytesToWrite > 0) {
//        // 每次发送payloadSize大小的数据，这里设置为64KB，如果剩余的数据不足64KB，
//        // 就发送剩余数据的大小
//        outBlock = localFile->read(qMin(bytesToWrite, payloadSize));
//        // 发送完一次数据后还剩余数据的大小
//        bytesToWrite -= (int)tcpClient->write(outBlock);
//        // 清空发送缓冲区
//        outBlock.resize(0);
//    } else { // 如果没有发送任何数据，则关闭文件
//        localFile->close();
//    }
//    // 更新进度条
//    ui->clientProgressBar->setMaximum(totalBytes);
//    ui->clientProgressBar->setValue(bytesWritten);
//    // 如果发送完毕
//    if(bytesWritten == totalBytes) {
//        ui->clientStatusLabel->setText(tr("传送文件 %1 成功").arg(fileName));
//        localFile->close();
//	emit done();
//        tcpClient->close();
//    }
}

// 显示错误
void Client::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(tr("客户端就绪"));
    ui->sendButton->setEnabled(true);
}



