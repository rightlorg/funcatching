#include "game.h"
#include <QGLWidget>
#include <QChar>

Game::Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
           QString mapPath, int gametype):
    QObject(parent_readypage)
{
    readypage = parent_readypage;
    mainwindow  = parent_mainwindow;

    scene = new QGraphicsScene(mainwindow);
    view = new QGraphicsView(scene, mainwindow);
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    mainwindow->addviewWidget(view);
    initSceneBackground();

    loadTexture();

    if(gametype == SinglePlayer) {
        map = new Map(NULL, mapPath);
        if(!map->loadMap()) {
            readypage->back();
            delete this;
        }
        initBlock();
        initPlayer();
    } else {
        connectServer();
        //		connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
    }
}

Game::~Game()
{
    delete map;
}

//bool Game::genHeadPic(QImage image, Camp camp, QString playerName)
//{
//}

void Game::initSceneBackground()
{
    //	scene->setForegroundBrush(QColor(200, 255, 255));
    scene->setBackgroundBrush(QPixmap(":/image/pix3.png"));
    //	scene->setSceneRect(-100, -100, 200, 200);
}

void Game::getHeadPic()
{
    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("HeadImage");
    QString path = settings.value("head_image", "").toString();
    QFile a(path);
    if(!a.exists())
    {
        return;
    }
    settings.endGroup();
    headImage =  new QImage;
    headImage->load(path);
}

void Game::connectServer()
{
    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("IP Address");
    QHostAddress *address = new QHostAddress(settings.value("IP").toString());
    qDebug()<<"asdf";
    tcpSocket.connectToHost(*address,2048);
    settings.endGroup();
    //example
    storing_player A;
    storing_player B;
    A.x = 0;    A.y = 0;    A.z = 0;
    B.x = 0;    B.y = 0;    B.z = 0;
    player.insert(player_index++,&A);
    player.insert(player_index++,&B);
    bool a = (*player[0]==*player[1]);
    qDebug()<<a;
}

void Game::initBlock()
{
    int rowsize = map->mapRowSize(0);
    int columnsize = map->mapRowSize(0);
    QPixmap tex(":/tex/3.png");
    for(int i = 0; i < rowsize; i++) {
        for (int j = 0; j < columnsize; j++) {
            QGraphicsPixmapItem *block = new QGraphicsPixmapItem(tex);
            block->setPos(32 * j, 32 * i);
            scene->addItem(block);
        }
    }
}

void Game::initPlayer()
{

}

void Game::loadTexture()
{
    QDir dir(":/tex/");
    dir.setSorting(QDir::Name);
    QStringList list, filter;
    filter << "*.png";
    dir.setNameFilters(filter);
    list = dir.entryList();
    qDebug() << list;
    qDebug() << dir.absoluteFilePath(list[0]);

    QString tmp_str;
    QChar tmp_c;
    int i = 0, previous = 0;
    {
        QList<QPixmap> newlist;
        texture.append(newlist);
    }
    foreach (QString texname, list) {
        tmp_c = texname[0];
        tmp_str = QString(tmp_c);
        i = tmp_str.toInt();
        if (previous != i) {
            QList<QPixmap> newlist;
            texture.append(newlist);
            previous = i;
        }
        texture[previous].append(QPixmap(dir.absoluteFilePath(texname)));
    }
}

void Game::firstDataSubmit()
{
    qDebug()<<"asd";
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);

    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("Player Name");
    player_name = settings.value("name").toString();
    settings.endGroup();

    out.setVersion(QDataStream::Qt_4_8);
    out<<quint32(0)<<player_name;//<<headImage;
    out.device()->seek(0);
    out<<quint32(block.size()-sizeof(quint16));
    tcpSocket.write(block);

    disconnect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
}

void initHeadPic()
{

}
