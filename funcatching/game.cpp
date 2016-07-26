#include "game.h"
#include <QGLWidget>
#include <QChar>
#include "gamemenu.h"
#include <QBuffer>

#define GAME_TICK 20	//Update n times per second

Game::Game(ReadyPage *parent_readypage, MainWindow *parent_mainwindow,
           QString mapPathTemp, int gameTypeTemp):
        QObject(parent_readypage)
{

	moveDown	= false;
	moveLeft	= false;
	moveUp		= false;
	moveRight	= false;
	finalMoveDown	= false;
	finalMoveLeft	= false;
	finalMoveUp	= false;
	finalMoveRight	= false;

	floor = 0;

	gameType	= gameTypeTemp;
	mapPath		= mapPathTemp;
	player_index	= 0;
	readypage	= parent_readypage;
	mainwindow	= parent_mainwindow;
	//	scene		= new QGraphicsScene(mainwindow);

	//	view		= new QGraphicsView(scene, mainwindow);
	view.setScene(&scene);
	view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	view.setFocusPolicy(Qt::StrongFocus);
	mainwindow->addviewWidget(&view);
	QPixmap white(":/image/white.png");
	white = white.scaled(27, 27, Qt::IgnoreAspectRatio);
	collisionCheckBlock.setPixmap(white);
	scene.addItem(&collisionCheckBlock);
	initSceneBackground();
	connect(&gameTick, SIGNAL(timeout()), SLOT(timerUpdate()));
	loadTexture();

	scene.installEventFilter(this);
	view.setFocus();

}

void Game::exitGame()
{
	readypage->back();
	gameTick.stop();
	delete map;
	//	delete scene;
	//	delete view;
	delete myself;
	delete this;
}


Game::~Game()
{

}

bool Game::loadMap()
{
	if(gameType == SinglePlayer) {
		map = new Map(NULL, mapPath);
		if (!map->readInitMapFile()) {
			return false;
			//			readypage->back();
			//		delete this;
		}
		if(!map->loadMap()) {
			return false;
			//			readypage->back();
			//            delete this;
		}
		//		paintBlocks(0);
		//		initPlayer(SinglePlayer);
	}/*else {
		connectServer();
		connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));
		connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(getFirst()));
	}*/
	return true;
}

//bool Game::isInitOK()
//{
//	return loadSuccess;
//}

bool Game::eventFilter(QObject *object, QEvent *event)
{
	switch (event->type()) {
	case QEvent::KeyPress:
		whenKeyPressed((QKeyEvent *)event);
		break;
	case QEvent::KeyRelease:
		whenKeyReleased((QKeyEvent *)event);
		break;
	default:
		return QObject::eventFilter(object, event);
	}
	return true;
}

void Game::initSceneBackground()
{
	scene.setBackgroundBrush(QPixmap(":/image/pix3.png"));
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
			return;
		settings.endGroup();
		myself_headImage.load(path);
		myself_headImage = myself_headImage.scaled(27, 27, Qt::IgnoreAspectRatio);
		break;
	}
	case Multiplayer:
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
	connect(&tcpSocket,SIGNAL(connected()),this,SLOT(firstDataSubmit()));

}

void Game::paintBlocks(int localFloor)
{
	int rowsize = map->mapRowSize(localFloor);
	int columnsize = map->mapRowSize(localFloor);
	int shadowStyle = 8;
	for(int i = 0; i < rowsize; i++) {
		for (int j = 0; j < columnsize; j++) {
			if (map->at(j, i, localFloor).id == 0)
				continue;


			//Paint stair
			if (map->at(j, i, localFloor).id >= 202
			        && map->at(j, i, localFloor).id <= 209) {
				QGraphicsPixmapItem *block = new QGraphicsPixmapItem(
				                        specialTex[map->at(j, i, localFloor).id - 200]);
				block->setPos(32 * j, 32 * i);
				block->setData(66, map->blockStatus(j, i, localFloor));
				scene.addItem(block);
				continue;
			}

			//common blocks
			shadowStyle = map->findWall(j, i, localFloor);
			if (map->at(j, i, localFloor).id > texture.size())
				continue;
			if (shadowStyle == -1)
				shadowStyle = texture[map->at(j, i, localFloor).id].size() - 1;
			QGraphicsPixmapItem *block = new QGraphicsPixmapItem(
			                        texture[map->at(j, i, localFloor).id][shadowStyle]);
			block->setPos(32 * j, 32 * i);
			block->setData(66, map->blockStatus(j, i, localFloor));
			scene.addItem(block);
		}
	}
	scene.setSceneRect(map->getSpawnPoint(0).rx() * 32, map->getSpawnPoint(0).ry() * 32,
	                   32, 32);
}

void Game::initPlayer(int gametype)
{
	QPoint spawnPoint = map->getSpawnPoint(0);
	switch (gametype) {
	case SinglePlayer:
		getHeadPic(SinglePlayer);
		myself = new QGraphicsPixmapItem(myself_headImage);
		myself->setPos(spawnPoint.rx() * 32 + 3, spawnPoint.ry() * 32 + 3);
		scene.addItem(myself);
		break;
	case Multiplayer:
		for (int i = 0; i < player_headImages.size(); ++i) {
			QGraphicsPixmapItem *newplayer = new QGraphicsPixmapItem(player_headImages[i]);
			newplayer->setPos(spawnPoint.rx() * 32 + 3, spawnPoint.ry() * 32 + 3);
			scene.addItem(newplayer);
		}

		break;
	default:
		return;
	}


}

void Game::initGame()
{
	paintBlocks(0);
	initPlayer(gameType);

	gameTick.start(1000 / GAME_TICK);

}

void Game::reMoveAllBlocks()
{
	QList<QGraphicsItem *> itemList = scene.items();
	for (int i = 0; i < itemList.size(); ++i) {
		scene.removeItem(itemList[i]);
	}
}

void Game::loadTexture()
{
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


	//Load special tex
	{
		QDir dir(":/item/image/");
		QString tmp_str;
		QStringList list, filter;

		specialTex.append(QPixmap());specialTex.append(QPixmap());	//first two are empty
		list = dir.entryList();
		qDebug() << list;
		dir.setNameFilters(QStringList("*.png"));
		for (int i = 0; i < list.size(); ++i) {
			dir.cd(list[i]);

			QStringList texname = dir.entryList();
			qDebug() << texname;
			for (int j = 0; j < texname.size(); ++j) {
				specialTex.append(QPixmap(dir.absoluteFilePath(texname[j])).scaled(32, 32));
			}
		}
	}
}

void Game::movePlayer(bool up, bool down, bool left, bool right)
{
	if (down)
		setYPos(PACE);
	if (up)
		setYPos(-PACE);
	if (left)
		setXPos(-PACE);
	if (right)
		setXPos(PACE);
}

void Game::whenKeyPressed(QKeyEvent *event)
{
	qDebug() << myself->pos();
	if (event->isAutoRepeat())
	{
		event->ignore();
		return;
	}

	switch (event->key()) {
	case Qt::Key_Up:
	case Qt::Key_W:
		//		setYPos(-PACE);
		finalMoveUp = true;
		break;
	case Qt::Key_Left:
	case Qt::Key_A:
		//		setXPos(-PACE);
		finalMoveLeft = true;
		break;
	case Qt::Key_Down:
	case Qt::Key_S:
		//		setYPos(PACE);
		finalMoveDown = true;
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
		//		setXPos(PACE);
		finalMoveRight = true;
		break;
	case Qt::Key_E:
		//add something here
		break;
	default:
		break;
	}

}

void Game::whenKeyReleased(QKeyEvent *event)
{
	qDebug("release");
	if (event->isAutoRepeat())
	{
		event->ignore();
		return;
	}

	switch (event->key()) {
	case Qt::Key_Up:
	case Qt::Key_W:
		//		setYPos(-PACE);
		finalMoveUp = false;
		break;
	case Qt::Key_Left:
	case Qt::Key_A:
		//		setXPos(-PACE);
		finalMoveLeft = false;
		break;
	case Qt::Key_Down:
	case Qt::Key_S:
		//		setYPos(PACE);
		finalMoveDown = false;
		break;
	case Qt::Key_Right:
	case Qt::Key_D:
		//		setXPos(PACE);
		finalMoveRight = false;
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
	QBuffer buffer;
	QDataStream out(&block,QIODevice::WriteOnly);

	QSettings settings("Funcatching Project", "Funcatching");
	settings.beginGroup("Player Name");
	player_name = settings.value("name").toString();
	settings.endGroup();

	buffer.open(QIODevice::ReadWrite);
	myself_headImage.toImage().save(&buffer, "PNG");

	out.setVersion(QDataStream::Qt_4_8);
	out << quint32(0) << player_name;
	out << buffer.data();
	out.device()->seek(0);
	out<<quint32(block.size()-sizeof(quint32));
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
		storing_player *newPlayer = new storing_player;
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

void Game::timerUpdate()
{
	//	qDebug("timer");
	//	QGraphicsPixmapItem *hit
	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
	haveWallA = false;
	haveWallB = false;

	QList<QGraphicsItem*> hits = myself->collidingItems(Qt::IntersectsItemBoundingRect);
	QList<QGraphicsItem*> hitsChecking;
	QList<QGraphicsItem*>::iterator it, itChecking;

	for (it = hits.begin(); it < hits.end(); ++it) {
		//check stair
		switch ((*it)->data(66).toInt()) {
		case 2:
		case 3:
		case 4:
		case 5:
		{
			if (floor + 1 < map->maxFloorSize()) {
				reMoveAllBlocks();
				floor += 1;
				paintBlocks(floor);
				initPlayer(gameType);
				myself->setPos(scene.sceneRect().x(), scene.sceneRect().y());

			}
			break;
		}
		case 6:
		case 7:
		case 8:
		case 9:
		{
			if (floor - 1 >= 0) {
				reMoveAllBlocks();
				floor -= 1;
				paintBlocks(floor);
				initPlayer(gameType);
				myself->setPos(scene.sceneRect().x(), scene.sceneRect().y());

			}

			break;
}


		}



		if ((*it)->data(66) == 1) {
			//MOVE RIGHT
			haveWallA = false;
			haveWallB = false;
			collisionCheckBlock.setPos(myself->x() + 1, myself->y());
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					moveRight = false;
				}
			}
			collisionCheckBlock.setPos(myself->x() + 1, myself->y() - 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallA = true;
					break;
				}
			}
			//			if (finalMoveRight == true) {
			//				moveRight = !haveWall;
			//				goto A;
			//			}
			//			haveWall = false;

			collisionCheckBlock.setPos(myself->x() + 1, myself->y() + 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallB = true;
					break;

				}
			}
			if (finalMoveRight == true) {
				moveRight = !(haveWallA && haveWallB);
			}
			//			A:
			haveWallA = false;
			haveWallB = false;

			//MOVE LEFT
			collisionCheckBlock.setPos(myself->x() - 1, myself->y());
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					moveLeft = false;
				}
			}
			collisionCheckBlock.setPos(myself->x() - 1, myself->y() - 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallA = true;
					break;
				}
			}
			//			if (finalMoveLeft == true) {
			//				moveLeft = !haveWall;

			//				goto B;
			//			}
			//			haveWall = false;
			collisionCheckBlock.setPos(myself->x() - 1, myself->y() + 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallB = true;
					break;

				}
			}
			if (finalMoveLeft == true) {
				moveLeft = !(haveWallA && haveWallB);
			}
			haveWallA = false;
			haveWallB = false;
			//			B:
			//MOVE DOWN
			collisionCheckBlock.setPos(myself->x(), myself->y() + 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					moveDown = false;
				}
			}
			collisionCheckBlock.setPos(myself->x() + 1, myself->y() + 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallA = true;
					break;
				}
			}
			//			if (finalMoveDown == true) {
			//				moveDown = !haveWall;

			//				goto C;
			//			}
			//			haveWall = false;
			collisionCheckBlock.setPos(myself->x() - 1, myself->y() + 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallB = true;
					break;

				}
			}
			if (finalMoveDown == true) {
				moveDown = !(haveWallA && haveWallB);
			}
			haveWallA = false;
			haveWallB = false;


			//			C:
			//MOVE UP
			collisionCheckBlock.setPos(myself->x(), myself->y() - 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					moveUp = false;
				}
			}

			collisionCheckBlock.setPos(myself->x() + 1, myself->y() - 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallA = true;
					break;
				}
			}
			//			if (finalMoveUp == true) {
			//				moveUp = !haveWall;

			//				goto D;
			//			}
			//			haveWall = false;

			collisionCheckBlock.setPos(myself->x() - 1, myself->y() - 1);
			hitsChecking = collisionCheckBlock.collidingItems(Qt::IntersectsItemBoundingRect);
			for (itChecking = hitsChecking.begin(); itChecking < hitsChecking.end(); ++itChecking) {
				if ((*itChecking)->data(66) == 1) {
					haveWallB = true;
					break;
				}
			}
			if (finalMoveUp == true) {
				moveUp = !(haveWallA && haveWallB);
			}
			haveWallA = false;
			haveWallB = false;
			//			D:
			movePlayer(moveUp, moveDown, moveLeft, moveRight);
			return;
			//			finalMoveUp = moveUp;
			//			finalMoveDown = moveDown;
			//			finalMoveLeft = moveLeft;
			//			finalMoveRight = moveRight;


			//			if (myself->x() > (*it)->x()) {
			//				if (myself->x() - (*it)->x() <= 32) {
			//					moveLeft = false;
			//				}
			//			}
			//			if (myself->x() < (*it)->x()) {
			//				if ((*it)->x() - myself->x()<= 32) {
			//					moveRight = false;
			//				}
			//			}
			//			if (myself->y() > (*it)->y()) {
			//				if (myself->y() - (*it)->y() <= 32) {
			//					moveUp = false;
			//				}
			//			}
			//			if (myself->y() < (*it)->y()) {
			//				if ((*it)->y() - myself->y()<= 32) {
			//					moveDown = false;
			//				}
			//			}

			qDebug("hitwall");
		}
	}
	//	qDebug();
	movePlayer(finalMoveUp, finalMoveDown, finalMoveLeft, finalMoveRight);

}

void Game::setXPos(int x_pos)
{
	myself->setPos(myself->pos().rx() + x_pos, myself->pos().ry());
	scene.setSceneRect(myself->pos().rx(), myself->pos().ry(), 32, 32);
}

void Game::setYPos(int y_pos)
{
	myself->setPos(myself->pos().rx(), myself->pos().ry() + y_pos);
	scene.setSceneRect(myself->pos().rx(), myself->pos().ry(), 32, 32);
}
