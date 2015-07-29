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
#include	<QDir>
#include <QMessageBox>
#include <QDebug>

typedef QList<QList<quint8> > DoubleQuintList;

class Map : public QObject
{
	Q_OBJECT
public:
	explicit Map(QObject *parent = 0, QString path = "");
	~Map();
	bool saveMap();
	bool loadMap();
	QString at(int x, int y, int z);
	bool change(int x, int y, int z);
	int mapRowSize(int floor);
	int mapColumnSize(int floor);
signals:

public slots:

private:
	QList<DoubleQuintList> map;		//相当于三维数组
	enum {MagicNum = 0x2FA3120C};
	QStringList floorPath;					//每层地图文件名
	quint32 spawn_row, spawn_column;
};

#endif // MAP_H
