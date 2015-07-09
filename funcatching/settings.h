#ifndef SETTTINGS_H
#define SETTTINGS_H

#include <QDialog>
#include <QLabel>
#include <QToolBar>
#include <QMenuBar>
#include "ui_settings.h"

namespace Ui {
class settings;
}

class settings : public QDialog
{
	Q_OBJECT
public:
	explicit settings(QWidget *parent = 0);
	~settings();

private slots:
	void on_backButton_clicked();
	void on_mapButton_clicked();
	void on_setpicButton_clicked();

private:
	Ui::settings *ui;
	QString filename;
};

#endif // SETTTINGS_H
