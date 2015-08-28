#include "game.h"
#include <QGLWidget>
#include <QChar>
#include "gamemenu.h"

Game::Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
           QString mapPath, int gametype):
    QObject(parent_readypage)
{
    player_index = 0;
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
        paintBlocks(0);
        initPlayer(SinglePlayer);
    }else {
        connectServer();
        connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
        connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(getFirst()));
    }
    scene->installEventFilter(this);
}

void Game::exitGame()
{
	//	map->saveMap();
	delete this;
	readypage->show();
}


Game::~Game()
{
    delete map;
	delete scene;
	delete view;
}

bool Game::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}

void Game::initSceneBackground()
{
    //	scene->setForegroundBrush(QColor(200, 255, 255));
    scene->setBackgroundBrush(QPixmap(":/image/pix3.png"));
}

void Game::getHeadPic(int gametype)
{
    switch (gametype) {
    case SinglePlayer:
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
        myself_headImage.load(path);
        myself_headImage = myself_headImage.scaled(26, 26, Qt::IgnoreAspectRatio);
    }
        break;
    case Multiplayer:
        //		player_headImages.append(headImage);
        break;
    default:
        break;
    }
}

void Game::connectServer()
{
    QSettings settings("Funcatching Project", "Funcatching");
    settings.beginGroup("IP Address");
    QHostAddress *address = new QHostAddress(settings.value("IP").toString());
    tcpSocket.connectToHost(*address,2048);
    settings.endGroup();
}

void Game::paintBlocks(int floor)
{
    int rowsize = map->mapRowSize(0);
    int columnsize = map->mapRowSize(0);
    int shadowStyle = 8;
    //    qDebug() << rowsize << columnsize;
    for(int i = 0; i < rowsize; i++) {
        for (int j = 0; j < columnsize; j++) {
            if (map->at(j, i, floor).id == 0)
                continue;
            shadowStyle = map->findWall(j, i, floor);
            if (map->at(i, j, floor).id > texture.size())
                continue;
            if (shadowStyle == -1)
                shadowStyle = texture[map->at(j, i, floor).id].size() - 1;
            QGraphicsPixmapItem *block = new QGraphicsPixmapItem(
                        texture[map->at(j, i, floor).id]
                    [shadowStyle]);

            block->setPos(32 * j, 32 * i);
            scene->addItem(block);
        }
    }
    scene->setSceneRect(map->getspawnPoint(0).rx() * 32, map->getspawnPoint(0).ry() * 32,
                        32, 32);
    //    scene->itemAt();
    //    scene->setSceneRect(0,0,0,0);
}

void Game::initPlayer(int gametype)
{
    QPoint spawnPoint = map->getspawnPoint(0);
    switch (gametype) {
    case SinglePlayer:
        getHeadPic(SinglePlayer);
        myself = new QGraphicsPixmapItem(myself_headImage);
        myself->setPos(spawnPoint.rx() * 32 + 3, spawnPoint.ry() * 32 + 3);
        scene->addItem(myself);
        break;
    case Multiplayer:
        for (int i = 0; i < player_headImages.size(); ++i) {
            QGraphicsPixmapItem *newplayer = new QGraphicsPixmapItem(player_headImages[i]);
            newplayer->setPos(spawnPoint.rx() * 32 + 3, spawnPoint.ry() * 32 + 3);
            scene->addItem(newplayer);
        }

        break;
    default:
        return;
    }


}

void Game::loadTexture()
{
    QDir dir(":/tex/");
    QString tmp_str;
    QStringList list, filter;

    filter << "*.png";
    dir.setNameFilters(filter);
    list = dir.entryList();

    //对材质列表按名称排序
    {
        int a = 0, b = 0, length = list.size(), m = length;
        for (int i = 1; i < length; ++i) {
            m -= 1;
            for (int j = 0; j < m; ++j) {
                tmp_str = QString(list[j][0]);
                if (list[j][1].toAscii() == '1' || list[j][1].toAscii() == '2' ||
                        list[j][1].toAscii() == '3' || list[j][1].toAscii() == '4' ||
                        list[j][1].toAscii() == '5' || list[j][1].toAscii() == '6' ||
                        list[j][1].toAscii() == '7' || list[j][1].toAscii() == '8' ||
                        list[j][1].toAscii() == '9' || list[j][1].toAscii() == '0')
                    tmp_str.append(list[j][1]);
                a = tmp_str.toInt();

                tmp_str = QString(list[j + 1][0]);
                if (list[j + 1][1].toAscii() == '1' || list[j + 1][1].toAscii() == '2' ||
                        list[j + 1][1].toAscii() == '3' || list[j + 1][1].toAscii() == '4' ||
                        list[j + 1][1].toAscii() == '5' || list[j + 1][1].toAscii() == '6' ||
                        list[j + 1][1].toAscii() == '7' || list[j + 1][1].toAscii() == '8' ||
                        list[j + 1][1].toAscii() == '9' || list[j + 1][1].toAscii() == '0')
                    tmp_str.append(list[j + 1][1]);
                b = tmp_str.toInt();
                if (a > b) {
                    tmp_str = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = tmp_str;
                }
            }
        }
    }
    //载入材质
    int i = 0, previous = 0;
    {
        QList<QPixmap> newlist;
        texture.append(newlist);
    }
    foreach (QString texname, list) {

        tmp_str = QString(texname[0]);
        if (texname[1].toAscii() == '1' || texname[1].toAscii() == '2' || texname[1].toAscii() == '3' || texname[1].toAscii() == '4' || texname[1].toAscii() == '5' || texname[1].toAscii() == '6' || texname[1].toAscii() == '7' || texname[1].toAscii() == '8' || texname[1].toAscii() == '9' || texname[1].toAscii() == '0') {
            tmp_str.append(texname[1]);
        }

        i = tmp_str.toInt();
        if (previous != i) {
            QList<QPixmap> newlist;
            texture.append(newlist);
            previous = i;
        }
        texture[previous].append(QPixmap(dir.absoluteFilePath(texname)));
    }
}

void Game::handleKeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        change_y_pos(-30);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        change_x_pos(-30);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        change_y_pos(30);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        change_x_pos(-30);
        break;
    case Qt::Key_Escape:
	gameMenu();
        break;
    case Qt::Key_E:
        //add something here
        break;
    default:
        break;
    }
}

void Game::firstDataSubmit()
{
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

void Game::getFirst()
{
    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_4_8);

    while(nextBlockSize==0){
        in >> nextBlockSize;
        if(nextBlockSize<tcpSocket.bytesAvailable())
            break;
    }
    unsigned short player_index;
    bool identity;
    unsigned short x,y,z;
    QString player_name;
    QImage *player_image = new QImage;

    in >> player_index;
    for(int i=0;i<=player_index;i++){
        in >>/* player_image >>*/ player_name >>identity >> x >> y >> z;
	storing_player *newPlayer;
        newPlayer->identety = identity;
        newPlayer->x = x;
        newPlayer->y = y;
        newPlayer->z = z;
        player.insert(i,newPlayer);
        //        player_headImages->append(player_image);
    }
}

void Game::gameMenu()
{
	GameMenu *gamemenu = new GameMenu(NULL, this);
	gamemenu->exec();
}

inline void Game::change_x_pos(int x_pos)
{
    myself->setPos(myself->pos().rx(), myself->pos().ry() + x_pos);
    scene->setSceneRect(myself->pos().rx(), myself->pos().ry(), 32, 32);
}

inline void Game::change_y_pos(int y_pos)
{
    myself->setPos(myself->pos().rx(), myself->pos().ry() + y_pos);
    scene->setSceneRect(myself->pos().rx(), myself->pos().ry(), 32, 32);
}

