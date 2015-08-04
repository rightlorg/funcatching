#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <QString>
#include <QHash>
#include <QMap>
#include <QObject>
#include <QString>



class BlockList : public QObject
{
	Q_OBJECT

public:
	BlockList(QObject *parent = 0);
//	quint8 getValue(QString key);
//	QString getKey(quint8 value);
    QHash<QString, quint8> blocklist;
	~BlockList();
private:
};

extern BlockList blocklist;

#endif // BLOCKLIST_H
