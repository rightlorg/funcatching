#ifndef MAP_H
#define MAP_H
#include <QDialog>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>
#include "ui_map.h"

namespace Ui {
        class mapEditor;
}

class mapEditor : public QWidget
{
                Q_OBJECT
 public:
    explicit mapEditor(QWidget *parent = 0);
    ~mapEditor();
private slots:
    void openFile();
    void saveFile();
    void quitFile();
    void newFile();
private:
    void createToolBar();
    void createMenuBar();
    void createStatusBar();
    QString filename;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *toolMenu;
    //menubar  action
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionQuit;
    Ui::mapEditor *ui;
};
#endif // MAP_H
