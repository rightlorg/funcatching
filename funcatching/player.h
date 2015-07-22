#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QSettings>
#include <QGraphicsPixmapItem>


class Player : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	explicit Player(QObject *parent = 0, QString *name = 0, QImage *image = 0);
	void readHeadImage();

signals:

public slots:

private:
	QImage *headImage;
	QString playerName;

};

#endif // PLAYER_H
