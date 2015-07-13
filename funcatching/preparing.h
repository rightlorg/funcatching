#ifndef PREPARING_H
#define PREPARING_H

#include <QDir>
#include <QPixmap>

enum Camp
{
	Catcher,
	Hider,
	None
};

extern void genHeadPic(QPixmap image, Camp camp);

#endif // PREPARING_H
