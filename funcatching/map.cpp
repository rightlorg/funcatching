#include "map.h"

Map::Map(QObject *parent, QString path) :
	QObject(parent)
{
	//读取每层地图文件名
	QDir dir;
	dir.cd("map");
	dir.cd(path);
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
		out << MagicNum;

		quint32 totalColumn = map[i].size();
		quint32 totalRow = map[i][0].size();

		out << totalRow;
		out << totalColumn;

		for(quint32 rowIndex = 0; rowIndex < totalRow; rowIndex++)
		{
			for(quint32 columnIndex = 0; columnIndex < totalColumn; totalColumn++)
			{
				out << rowIndex << columnIndex;
				out << map[i][rowIndex][columnIndex];
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
		QList<QStringList> newFloor;
		map.append(newFloor);


		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_8);
		quint32 magic;
		in >> magic;
		if(magic!=MagicNum){
			QMessageBox::warning(NULL,tr("Map editor"),
					     tr("This file is not a Map file\nPlease rechoose the map"));
			return false;
		}

		quint32 totalColumn = 0;
		int columnIndex = 0;
		int rowIndex = 0;
		QString str;
		{
			quint32 tmp;
			in >> tmp;				//抛弃垃圾值
		}
		quint16 tmp;
		in >> totalColumn;
		{
			QStringList newRow;
			map[i].append(newRow);
		}
		while(!in.atEnd())
		{
			//检查是否要换行
			if(columnIndex == totalColumn)
			{
				QStringList newRow;
				map[i].append(newRow);
				rowIndex++;
				columnIndex = 0;
			}
			in >> tmp >> tmp >> str;			//抛弃垃圾值
			qDebug() << str;
			map[i][rowIndex].append(str);
			columnIndex++;
		}
		file.close();
	}
	return true;
}

QString Map::at(int x, int y, int z)
{

}
