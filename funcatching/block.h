#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>

class Block : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Block();
};

#endif // BLOCK_H
