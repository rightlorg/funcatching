#include "blocklist.h"

BlockList::BlockList(QObject *parent) : QObject(parent)
{
	blocklist[tr("Sand")] = 0;
	blocklist[tr("Stone")] = 1;
	blocklist[tr("Vertical Glass Pane")] = 2;
	blocklist[tr("Oak Wood")] = 3;
	blocklist[tr("Glass")] = 4;
	blocklist[tr("Birch")] = 5;
	blocklist[tr("Mushroom")] = 6;
//	blocklist[tr("")] = 7;
//	blocklist[tr("")] = 8;
//	blocklist[tr("")] = 9;
//	blocklist[tr("")] = 10;
//	blocklist[tr("")] = 11;
//	blocklist[tr("")] = 12;
//	blocklist[tr("")] = 13;
//	blocklist[tr("")] = 14;
//	blocklist[tr("")] = 15;
//	blocklist[tr("")] = 16;
//	blocklist[tr("")] = 17;
//	blocklist[tr("")] = 18;
//	blocklist[tr("")] = 19;
//	blocklist[tr("")] = 20;
//	blocklist[tr("")] = 21;
//	blocklist[tr("")] = 22;
//	blocklist[tr("")] = 23;
//	blocklist[tr("")] = 24;

}

quint8 BlockList::getValue(QString key)
{
	return blocklist.value(key);
}

QString BlockList::getKey(quint8 value)
{
	return blocklist.key(value);
}

BlockList::~BlockList()
{

}
