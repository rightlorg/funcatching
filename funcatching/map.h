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

struct MapImformations
{
	quint8 floor;
	QString filePath;
	QPoint spawnPoint;
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
	QPoint getSpawnPoint(int floor);
	bool isInitMap(QString filename);
	unsigned short blockStatus(int x, int y, int z);
	bool readIniMapFile();
	bool isMap(QString filename);
	bool saveIniMap(QString initmapPath, QList<MapImformations> inputMapImformation);
	QList<MapImformations> getMapImformations();
signals:

public slots:

private:
	QList<DoublemapBlockList> map;		//相当于三维数组
	enum {map_MagicNum = 0x2FA3120C, ini_MagicNum = 0x2FA3120D};
//	QStringList floorPath;					//每层地图文件名
	quint8 mapCount;//有多少个地图文件
	QList<MapImformations> mapImform; //每层地图的地图信
	QStringList iniMapFileName;


};

#endif // MAP_H
