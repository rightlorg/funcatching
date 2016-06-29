#include "movethread.h"

MoveThread::MoveThread(QObject *parent) : QThread(parent)
{
	game = (Game*)parent;
}

void MoveThread::run()
{
	while (true) {

	}
}
