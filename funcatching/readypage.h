#ifndef READYPAGE_H
#define READYPAGE_H

#include <QWidget>
#include <mainwindow.h>
#include <QModelIndex>
#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QInputDialog>
//#include <game.h>

namespace Ui {
class ReadyPage;
}

class ReadyPage : public QWidget
{
	Q_OBJECT

public:
	explicit ReadyPage(MainWindow *parent = 0);
	void addMap();
	void back();
	~ReadyPage();

private slots:
	void on_back_clicked();

	void on_listWidget_clicked(const QModelIndex &index);

	void on_go_clicked();

	void on_server_clicked();

private:
	bool haveHeadImage();
	Ui::ReadyPage *ui;
	MainWindow *mainwindow;
	int mapIndex;
	QDir dir;
	QStringList maps; //who store dir name of the map
	QString ip;
//	void *game;
};

#endif // READYPAGE_H
