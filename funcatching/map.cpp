#include "map.h"

Map::Map(QObject *parent, QString path) :
	QObject(parent)
{

}

Map::~Map()
{

}

bool Map::isMap(QDataStream mapStream)
{

}

bool Map::saveMap(QList<QStringList>)
{

}

QList<QStringList> Map::loadMap(QDataStream mapStream)
{

}

QString Map::at(int x, int y, int z)
{

}
