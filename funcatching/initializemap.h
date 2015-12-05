#ifndef INITIALIZEMAP_H
#define INITIALIZEMAP_H

#include <QMainWindow>
#include <QFile>
#include <QtGui>
#include <QMap>
#include "initialmap_stackmap.h"
#include "ui_initialmap_stackmap.h"
namespace Ui {
class initializemap;
}

class initializemap : public QMainWindow
{
    Q_OBJECT

public:
    explicit initializemap(QWidget *parent = 0);
    ~initializemap();

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_currentstack_directory_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    enum{map_MagicNum = 0x2FA3120C,ini_MagicNum = 0x2FA3120D};
    quint8 map_num;
    QMap<unsigned int, initialmap_stackmap*>total_stack_num;
    bool pos_checked;
    Ui::initializemap *ui;
};

#endif // INITIALIZEMAP_H
