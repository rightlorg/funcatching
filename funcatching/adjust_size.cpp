#include <QtGui>
#include "adjust_size.h"

adjust_size::adjust_size(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::adjust_size)
{
    ui->setupUi(this);
    _size = 0;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void adjust_size::on_horizontalSlider_valueChanged(int value)
{
    switch(value){
    case 0:
        ui->label->setText("very small");
        break;
    case 1:
        ui->label->setText("small");
        break;
    case 2:
        ui->label->setText("normal");
        break;
    case 3:
        ui->label->setText("big");
        break;
    case 4:
        ui->label->setText("very big");
        break;
    }
}
void adjust_size::on_buttonBox_accepted()
{
    this->hide();
}
