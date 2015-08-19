#ifndef MAP_H
#define MAP_H

//	C(y)
//	|
//	|
//	|
//	|
//------------------------------------>R(x)
//	|

#include <QObject>
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QStringList>
#include <QList>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QPoint>

struct mapBlcok
{
	quint8 id;
	quint8 status;
};

typedef QList<QList<mapBlcok> > DoublemapBlockList;

class Map : public QObject
{
	Q_OBJECT
public:
	explicit Map(QObject *parent = 0, QString path = "");
	~Map();
	bool saveMap();
	bool loadMap();
	mapBlcok at(int x, int y, int z);
	bool change(int x, int y, int z);
	int mapRowSize(int floor);
	int mapColumnSize(int floor);
	int findWall(int x, int y, int z);
	QPoint getspawnPoint(int floor);
signals:

public slots:

private:
	QList<DoublemapBlockList> map;		//相当于三维数组
    enum {map_MagicNum = 0x2FA3120C};
	QStringList floorPath;					//每层地图文件名
	QList<QPoint> spawnPoint;
};

#endif // MAP_H
