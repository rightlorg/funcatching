#ifndef READYPAGE_H
#define READYPAGE_H

#include <QWidget>
#include <mainwindow.h>

namespace Ui {
class ReadyPage;
}

class ReadyPage : public QWidget
{
	Q_OBJECT

public:
	explicit ReadyPage(MainWindow *parent = 0);
	void addMap();
	~ReadyPage();

private slots:
	void on_back_clicked();

private:
	Ui::ReadyPage *ui;
	MainWindow *mainwindow;
};

#endif // READYPAGE_H
