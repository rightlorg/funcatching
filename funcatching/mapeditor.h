#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QMainWindow>
#include <QtGui>
#include <QMap>
#include "adjust_size.h"
#include "ui_mapeditor.h"
#include <QList>
#include <QPushButton>

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
    //void closeEvent(QCloseEvent *event);

private slots:
    bool openFile();
    void saveFile();
    void quitFile();
    void dockDialog();
    void aboutFile();
    void ver();
    void add_new_row();
    void gotoCell();
    void bat_table();
    void adjust_table_size();
	void ondockbuttonClicked();
    void add_new_column();
    void onactionWall_clicded();
    void onactionNO_clicded();
    void on_nullButton_clicked();
    void on_tableWidget_clicked(const QModelIndex &index);
    void on_noButton_clicked();
    void on_wallButton_clicked();

private:
    typedef QMap<int, QMap<int, QLabel*> >all_label;
    enum{MagicNum = 0x2FA3120C};
    void createMenuBar();
    void createTableWidget(int a,int b);
    void createStatusBar();
    void add_one_label(int row, int column, QLabel* newlabel);
    void initialize_item(int row,int column,QString status);
    void initdockButtos();
    all_label storing_all_label;
    QMenuBar *menuBar;
    QLabel *statusLabel;
    QString itemstatusString;
    QLabel *label;
    QPixmap *statusImage;
    QString filename;
    int selection;
    int table_view_size;
    Ui::MapEditor *ui;

//    0:NO
//    1:Wall
//    ...:...
    int blockStatus;


    QList<QPushButton *> dockbuttonList;
};

#endif // MAPEDITOR_H
