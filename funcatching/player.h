#ifndef PLAYER_H
#define PLAYER_H

#include <QImage>
#include <QString>
#include <QSettings>
#include <QGraphicsPixmapItem>


class Player :public QGraphicsPixmapItem
{
public:
	Player(QString *name = 0);
	void readHeadImage();
	~Player();

	QPointF getRealPos() const;
	void setRealPos(const QPointF &value);

	quint8 getFloor() const;
	void setFloor(const quint8 &value);

	QString getPlayerName() const;

	QPixmap getHeadImage() const;
	void setHeadImage(const QPixmap &value);		//Must use this func when init a new player

private:
	QPixmap headImage;
	QPointF realPos;		//current pos were player at
	quint8 floor;		//current floor were player at
	QString playerName;
};

#endif // PLAYER_H
