#include <QDialog>
#include "ui_adjust_size.h"

namespace Ui {
class adjust_size;
}

class adjust_size : public QDialog, public Ui::adjust_size
{
    Q_OBJECT
public:
    adjust_size(QWidget *parent = 0/*,int table_size*/);
    unsigned short _size;
    Ui::adjust_size *ui;
private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_buttonBox_accepted();
};

