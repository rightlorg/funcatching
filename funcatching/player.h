#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPixmap>
#include <QString>
#include <QSettings>


class Player : public QObject
{
	Q_OBJECT
public:
	explicit Player(QObject *parent = 0, QString *name = 0, QPixmap *image = 0);
	void readHeadImage();

signals:

public slots:

private:
	QPixmap *headImage;
	QString playerName;

};

#endif // PLAYER_H
