#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QMainWindow>
#include <QtGui>
#include <ui_mapeditor.h>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
class QMenuBar;

namespace Ui {
class MapEditor;
}

class MapEditor : public QMainWindow
{
	Q_OBJECT

public:
	explicit MapEditor(QWidget *parent = 0);
	~MapEditor();
private slots:
    bool openFile();
	void saveFile();
	void quitFile();
	void newFile();
	void dockDialog();
	void aboutFile();
	void ver();
	void cell_paint(QTableWidgetItem* item);
	void add_new_row();

	void add_new_column();
	void on_VGlass_clicked();
	void on_HGlass_clicked();
	void on_VWall_clicked();
	void on_HWall_clicked();
	void on_VDoor_clicked();
	void on_HDoor_clicked();
	void on_Floor_clicked();

private:
	enum{MagicNum = 0x2FA3120C};
	void createMenuBar();
	void createTableWidget();
	void createStatusBar();
	QMenuBar *menuBar;
	QLabel *statusLabel;
	QLabel *itemstatusLabel;
	QImage *statusImage;
	QString filename;
	Ui::MapEditor *ui;
};

#endif // MAPEDITOR_H
