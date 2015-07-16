#include "map.h"

Map::Map(QObject *parent, QString path) :
	QObject(parent)
{
	mapPath  = path;
}

Map::~Map()
{

}

bool Map::saveMap(QList<QStringList>)
{
	return true;
}

bool Map::loadMap()
{
	//读取每层地图文件名
	QDir dir;
	dir.cd("map");
	dir.cd(mapPath);
	QStringList filter;
	filter << "*.map";
	dir.setNameFilters(filter);
	dir.setSorting(QDir::Name);
	floorPath << dir.entryList();
	//将文件名转换成文件的绝对路径
	for(int i = 0; i < floorPath.size(); i++)
	{
		floorPath[i] = dir.absoluteFilePath(floorPath[i]);
	}


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
			return NULL;
		}

		//添加新层
		QList<QStringList> newFloor;
		map.append(newFloor);


		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_8);
		quint32 magic;
		in >> magic;
		if(magic!=MagicNum){
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("This file is not a Map file\nPlease rechoose the map"));
			return NULL;
		}

		quint32 totalColumn;
		quint32 columnIndex;
		int rowIndex;
		QString str;
		in >> totalColumn;
		while(!in.atEnd())
		{
			//检查是否要换行
			if(columnIndex == totalColumn)
			{
				QStringList newRow;
				map.at(i).append(newRow);
				rowIndex++;
				columnIndex = 0;
			}
			in >> str;
			qDebug() << str;
			map.at(i).at(rowIndex).append(str);
			columnIndex++;

		}




	}
}

QString Map::at(int x, int y, int z)
{

}
