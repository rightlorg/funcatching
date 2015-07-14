#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>

#include "ui_gotocell.h"

namespace Ui {
class GoToCellDialog;
}

class GoToCellDialog : public QDialog, public Ui::GoToCellDialog
{
    Q_OBJECT

public:
    GoToCellDialog(QWidget *parent = 0);

private slots:
    void on_rowEdit_textChanged();
    void on_columnEdit_textChanged();

private:
    Ui::GoToCellDialog *ui;
};

#endif
