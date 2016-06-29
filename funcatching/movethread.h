#ifndef MOVETHREAD_H
#define MOVETHREAD_H

#include <QThread>
#include "game.h"

class MoveThread : public QThread
{
	Q_OBJECT
public:
	MoveThread(QObject *parent);
	void run();
private:
	Game *game;

};

#endif // MOVETHREAD_H
