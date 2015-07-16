#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QMainWindow>
#include <QtGui>
#include <ui_mapeditor.h>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QModelIndex>

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

protected:
    void closeEvent(QCloseEvent *event);

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
    void viewButton();
    void setColumnSize();
    void setRowSize();
    void gotoCell();
    void bat_table();

	void add_new_column();
	void on_VGlass_clicked();
	void on_HGlass_clicked();
	void on_VWall_clicked();
	void on_HWall_clicked();
	void on_VDoor_clicked();
	void on_HDoor_clicked();
	void on_Floor_clicked();
    void on_nullButton_clicked();

    void on_Clay_clicked();
    void on_Glass_clicked();
    void on_Wood_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

private:
	enum{MagicNum = 0x2FA3120C};
	void createMenuBar();
    void createTableWidget(int a,int b);
	void createStatusBar();
	QMenuBar *menuBar;
	QLabel *statusLabel;
	QLabel *itemstatusLabel;
    QPixmap *statusImage;
	QString filename;
	int selection;
	Ui::MapEditor *ui;
};

#endif // MAPEDITOR_H
