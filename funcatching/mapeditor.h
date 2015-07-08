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
   void openFile();
   void saveFile();
   void quitFile();
   void newFile();
   void dockDialog();
   void aboutFile();
   void ver();
   void cell_paint(QTableWidgetItem* item);
private:
   void createMenuBar();
   void createTableWidget();
   void createStatusBar();
   QMenuBar *menuBar;
   QLabel *statusLabel;
   QImage *statusImage = new QImage;
   QString filename;
   Ui::MapEditor *ui;
};

#endif // MAPEDITOR_H
