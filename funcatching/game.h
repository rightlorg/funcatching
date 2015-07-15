#ifndef GAME_H
#define GAME_H

#include <QString>
//#include <QPixmap>
#include <QCloseEvent>
#include <QObject>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QImage>
#include "map.h"

class Game : public QObject
{
    Q_OBJECT
public:
	explicit Game(QObject *parent, QString mapPath);
	~Game();

	enum Camp
	{
		Catcher,
		Hider,
		None
	};

public:
    void connectServer();
    void firstDataSubmit();

private:
	void genHeadPic(QImage image, Camp camp, QString playerName);
	QImage *getHeadPic(QString path);
	Map map;
};

#endif // GAME_H
