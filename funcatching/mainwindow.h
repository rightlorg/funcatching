#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	void showStartPage();
//	void hideWidget();
	void showWidget();
	void addviewWidget(QGraphicsView *view);
	~MainWindow();

private slots:
	void on_startGame_clicked();
	void on_settings_clicked();

private:
	void createStatusBar();
	QLabel *statusLabel;
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
