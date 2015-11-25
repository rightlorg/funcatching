#include "map.h"
#include <QDebug>

Map::Map(QObject *parent, QString path) :
	QObject(parent)
{
	//读取每层地图文件名
	qDebug(path);
	QDir dir(path);
	QStringList filter;
	filter << "*.initmap";
	dir.setNameFilters(filter);
	dir.setSorting(QDir::Name);
	floorPath << dir.entryList();
	//将文件名转换成文件的绝对路径
	for(int i = 0; i < floorPath.size(); i++)
	{
		floorPath[i] = dir.absoluteFilePath(floorPath[i]);
	}

}

Map::~Map()
{

}

bool Map::saveMap()
{
	QFile file;

	for(int i = 0; i < floorPath.size(); i++)
	{
		file.setFileName(floorPath[i]);
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
		{
			quint32 spawn_row = spawnPoint[i].ry(),
				spawn_column = spawnPoint[i].rx();
			out << spawn_row << spawn_column;
		}
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
	for(int i = 0; i < floorPath.size(); i++)
	{
		file.setFileName(floorPath[i]);
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
		quint32 magic;
		in >> magic;
        if(magic!=map_MagicNum){
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("This file is not a Map file\nPlease rechoose the map"));
			return false;
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
		//Get spawn point
		{
			quint32 spawn_row,spawn_column;
			in >> spawn_row >> spawn_column;
			spawnPoint.append(QPoint(spawn_column, spawn_row));

		}
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

QPoint Map::getspawnPoint(int floor)
{
	return QPoint(spawnPoint[floor].rx(), spawnPoint[floor].ry());
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

bool Map::readInitMapFile(QString filename) // Path is a initmap file name
{
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly))
	{
	       QMessageBox::warning(NULL,tr("Saving initial settings"),tr("failed to save settings"));
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

	MapImformations spawnT;			//T means temp
	quint32 spawnRowT, spawnColoumnT;
	QString mapFloorPath;
	for (int i = 0; i < tempMapCount; ++i) {
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
