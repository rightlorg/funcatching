#ifndef ADJUST_SIZE_H
#define ADJUST_SIZE_H

#include <QDialog>

namespace Ui {
class adjust_size;
}

class adjust_size : public QDialog
{
    Q_OBJECT

public:
    explicit adjust_size(QWidget *parent = 0);
    adjust_size(QWidget *parent = 0, int size = 0);
    ~adjust_size();
    int table_size;

private slots:
    void on_size_slider_valueChanged(int value);

private:
    Ui::adjust_size *ui;
};

#endif // ADJUST_SIZE_H
