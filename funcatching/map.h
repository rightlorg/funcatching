#ifndef MAP_H
#define MAP_H

//	Y
//	|
//	|
//	|
//	|
//------------------------------------>X
//	|

#include <QObject>
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QStringList>
#include <QList>

 typedef QList<QStringList> DoubleStringList;

class Map : public QObject
{
	Q_OBJECT
public:
	explicit Map(QObject *parent = 0, QString path = "");
	~Map();

	bool isMap(QDataStream mapStream);
	bool saveMap(QList<QStringList>);
	QList<QStringList> loadMap(QString mapStream);
	QString at(int x, int y, int z);

signals:

public slots:

private:
	QString mapPath;
	QList<DoubleStringList> map;	//相当于三维数组
};

#endif // MAP_H
