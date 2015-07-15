#include "map.h"

Map::Map(QObject *parent, QString path) :
	QObject(parent)
{
	mapPath  = path;
	map = new QList<DoubleStringList>;
}

Map::~Map()
{

}

bool Map::saveMap(QList<QStringList>)
{

}

QList<QStringList> *Map::loadMap()
{
	//读取每层地图文件名
	QDir dir;
	dir.cd("map");
	dir.cd(mapPath);
	QStringList filter;
	filter << "*.map";
	dir.setNameFilters(filter);
	dir.setSorting(QDir::Name);
	floors << dir.entryList();
	//将文件名转换成文件的绝对路径
	for(int i = 0; i < floors.size(); i++)
	{
		floors[i] = dir.absoluteFilePath(floors[i]);
	}


	QFile file;
	for(int i = 0; i < floors.size(); i++)
	{
		file.setFileName(floors[i]);
		if(!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("failed to read file %1:\n%2")
					     .arg(file.fileName())
					     .arg(file.errorString()));
			return NULL;
		}
		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_8);

		quint32 magic;
		quint32 temp_row;
		quint32 temp_column;

		in>>magic;

		if(magic!=MagicNum){
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("This file is not a Map file\nPlease rechoose the map"));
			return NULL;
		}
	}
}

QString Map::at(int x, int y, int z)
{

}
