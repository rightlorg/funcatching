#ifndef PREPARING_H
#define PREPARING_H

#include <QDir>
#include <QImage>
#include <QString>

enum Camp
{
	Catcher,
	Hider,
	None
};

extern void genHeadPic(QImage image, Camp camp);
extern  QImage getHeadPic(QString path);

#endif // PREPARING_H
