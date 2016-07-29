#include "map.h"
#include <QDebug>

Map::Map(QObject *parent, QString path) :
	QObject(parent)
{
	//读取每层地图文件名
	qDebug() << path;
	QDir dir(path);
	QStringList filter;
	filter << "*.initmap";
	dir.setNameFilters(filter);
	dir.setSorting(QDir::Name);
	mapCount = 0;
	initMapFileName << dir.entryList();
	initMapFileName[0] = dir.absoluteFilePath(initMapFileName[0]);
	//将文件名转换成文件的绝对路径
	//	for(int i = 0; i < floorPath.size(); i++)
	//	{
	//		floorPath[i] = dir.absoluteFilePath(floorPath[i]);
	//	}

}

Map::~Map()
{

}

bool Map::saveMap()
{
	QFile file;

	for(int i = 0; i < mapImform.size(); i++)
	{
		file.setFileName(mapImform[i].filePath);
		if(!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("failed to save file"));
			return false;
		}

		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_4_8);
		out << map_MagicNum;

		quint32 totalColumn = map[i].size();
		quint32 totalRow = map[i][0].size();

		out << totalRow;
		out << totalColumn;
		for(quint32 rowIndex = 0; rowIndex < totalRow; rowIndex++)
		{
			for(quint32 columnIndex = 0; columnIndex < totalColumn; totalColumn++)
			{
				out << rowIndex << columnIndex;
				out << map[i][rowIndex][columnIndex].id;
				out << map[i][rowIndex][columnIndex].status;
			}
		}
	}

	return true;
}

bool Map::loadMap()
{
	QFile file;
	for(int i = 0; i < mapImform.size(); i++)
	{
		if (!isMap(mapImform[i].filePath)) {
			return false;
		}

		file.setFileName(mapImform[i].filePath);
		if(!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("failed to read file %1:\n%2")
					     .arg(file.fileName())
					     .arg(file.errorString()));
			return false;
		}

		//添加新层
		DoublemapBlockList newFloor;
		map.append(newFloor);


		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_8);
		{
			quint32 magic;		//magic is garbage
			in >> magic;
		}
		quint32 totalColumn = 0;
		int columnIndex = 0;
		int rowIndex = 0;
		mapBlcok block;
		{
			quint32 tmp;
			in >> tmp;				//抛弃垃圾值
		}
		in >> totalColumn;
		{
			QList<mapBlcok> newRow;
			map[i].append(newRow);
		}
		quint32 tmp;
		while(!in.atEnd())
		{
			if(columnIndex == (int)totalColumn)			//检查是否要换行
			{
				QList<mapBlcok> newRow;
				map[i].append(newRow);
				rowIndex++;
				columnIndex = 0;
			}
			in >> tmp >> tmp;			//抛弃垃圾值
			in >> block.id >> block.status;
			map[i][rowIndex].append(block);
			columnIndex++;
		}
		file.close();
	}
	return true;
}

mapBlcok Map::at(int x, int y, int z)
{
	return map[z][y][x];
}

int Map::maxFloorSize()
{
	return map.size();
}

int Map::mapRowSize(int floor)
{
	return map[floor].size();
}

int Map::mapColumnSize(int floor)
{
	return map[floor][0].size();
}


// -1: No wall near by
// 0:right
// 1:left
// 2:bottom
// 3:top
// 4:bottom left
// 5:top right
// 6:top left
// 7:bottom right
int Map::findWall(int x, int y, int z)
{
	if (map[z][y][x].status == 1) {
		return -1;
	}
	//right&bottom&top
	if (x + 1 < map[z][y].size()) {
		if (map[z][y][x + 1].status == 1) {
			if (y + 1 < map[z].size())
				if (map[z][y + 1][x].status == 1)
					return 7;
			if (y - 1 >= 0)
				if (map[z][y - 1][x].status ==1)
					return 5;
			return 0;
		}
	}
	//left&bottom&top
	if (x - 1 >= 0) {
		if (map[z][y][x - 1].status == 1) {
			if (y + 1 < map[z].size())
				if (map[z][y + 1][x].status == 1)
					return 4;
			if (y - 1 >= 0)
				if (map[z][y - 1][x].status == 1)
					return 6;
			return 1;
		}
	}
	//top
	if (y - 1 >= 0)
		if (map[z][y -1][x].status == 1)
			return 3;
	//bottom
	if (y + 1 < map[z].size())
		if (map[z][y + 1][x].status == 1)
			return 2;
	return -1;
}

QPoint Map::getSpawnPoint(int floor)
{
	return QPoint(mapImform[floor].spawnPoint.rx(),
		      mapImform[floor].spawnPoint.ry());
}

bool Map::isInitMap(QString filename) // Path is a initmap file name
{
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)){
		QMessageBox::warning(NULL,tr("Map editor"),
				     tr("failed to read file %1:\n%2")
				     .arg(file.fileName())
				     .arg(file.errorString()));
		return false;
	}
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_8);

	quint32 magic;
	in >> magic;
	if(magic!=ini_MagicNum){
		return false;
	}
	file.close();
	return true;
}

unsigned short Map::blockStatus(int x, int y, int z)
{
	return map[z][y][x].status;
}

bool Map::readInitMapFile() // Path is a initmap file name
{
	QFile file(initMapFileName[0]);
	if(!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,tr("Saving initial settings"),tr("failed to read settings"));
		return false;
	}
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_8);
	{
		quint32 magicnum;
		in >> magicnum;
		if (magicnum != Map::ini_MagicNum) {
			return false;
		}
	}
	//How many maps are there
	quint8 tempMapCount;
	in >> tempMapCount;

	quint32 spawnRowT, spawnColoumnT;
	for (int i = 0; i < tempMapCount; ++i) {
		QString mapFloorPath;
		MapImformations spawnT;			//T means temp

		in >> mapFloorPath;
		if (isMap(mapFloorPath)) {
			spawnT.filePath.append(mapFloorPath);
			mapCount++;
		} else {
			quint32 garbage;
			in >> garbage >> garbage;
			continue;
		}
		in >> spawnRowT >> spawnColoumnT;
		spawnT.spawnPoint.setX(spawnColoumnT);
		spawnT.spawnPoint.setY(spawnRowT);
		spawnT.floor = mapCount - 1;
		mapImform.append(spawnT);
	}
	return true;
}

bool Map::isMap(QString filename) // Path is a map file name
{
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)){
		QMessageBox::warning(NULL,tr("Map editor"),
				     tr("failed to read file %1:\n%2")
				     .arg(file.fileName())
				     .arg(file.errorString()));
		return false;
	}
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_8);

	quint32 magic;
	in >> magic;
	if(magic!=map_MagicNum){
		return false;
	}
	file.close();
	return true;
}

//initmappath is a dirname and mapPath is a filename
bool Map::saveInitMap(QString initmapPath, QList<MapImformations> inputMapImformation)
{
	QFile file(initmapPath);
	bool ok = true;
	if(!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(NULL,tr("Saving initial settings"),tr("failed to save settings"));
		return false;
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_8);

	out << quint32(ini_MagicNum);
	out<< quint8(inputMapImformation.size());
	for(int i = 0; i < inputMapImformation.size(); i++) {
		if (isMap(inputMapImformation[i].filePath)) {
			ok = false;
			continue;
		}
		out << inputMapImformation[i].filePath;
		out << quint32(inputMapImformation[i].spawnPoint.ry())
		    << quint32(inputMapImformation[i].spawnPoint.ry());
	}
	return ok;
}

QList<MapImformations> Map::getMapImformations()
{
	return mapImform;
}

bool Map::sendMap(QTcpSocket *socket)
{
	
}

bool Map::downloadMapFromRemote(QTcpSocket *socket)
{
	
}
