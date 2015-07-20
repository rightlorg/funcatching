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

typedef QList<QStringList> DoubleStringList;

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

signals:

public slots:

private:
	QList<DoubleStringList> map;		//相当于三维数组
	enum {MagicNum = 0x2FA3120C};
	QStringList floorPath;					//每层地图文件名
};

#endif // MAP_H
