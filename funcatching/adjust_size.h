#ifndef ADJUST_SIZE_H
#define ADJUST_SIZE_H

#include <QDialog>
#include <QDebug>
#include "ui_adjust_size.h"

namespace Ui {
    class adjust_size;
}

class adjust_size : public QDialog
{
    Q_OBJECT

public:
    explicit adjust_size(QWidget *parent,int size);
    ~adjust_size();
    int table_size;

private slots:
    void on_size_slider_valueChanged(int value);
    void on_canceButton_clicked();
    void on_okButton_clicked();

private:
    void close_event();
    Ui::adjust_size *ui;
};

#endif // ADJUST_SIZE_H
