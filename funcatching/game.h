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
#include "readypage.h"

class Game : public QObject
{
    Q_OBJECT
public:
	explicit Game(ReadyPage *parent, QString mapPath);
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
	Map *map;
	ReadyPage *readypage;
};

#endif // GAME_H
