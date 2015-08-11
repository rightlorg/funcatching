#ifndef SETTTINGS_H
#define SETTTINGS_H

#include <QDialog>
#include <QLabel>
#include <QToolBar>
#include <QMenuBar>
#include "initializemap.h"
#include "ui_settings.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_backButton_clicked();
    void on_mapButton_clicked();
    void on_setpicButton_clicked();
    void on_setnameButton_clicked();

    void on_initialmapButton_clicked();

private:
    Ui::Settings *ui;
    QString filename;
};

#endif // SETTTINGS_H
