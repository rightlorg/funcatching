#include <QtGui>

#include "gotocell.h"

GoToCellDialog::GoToCellDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::GoToCellDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QRegExp regExp("[1-9][0-9]{0,2}");
    ui->columnEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->rowEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void GoToCellDialog::on_columnEdit_textChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            (ui->columnEdit->hasAcceptableInput())&&(ui->rowEdit->hasAcceptableInput()));
}

void GoToCellDialog::on_rowEdit_textChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            (ui->columnEdit->hasAcceptableInput())&&(ui->rowEdit->hasAcceptableInput()));
}

int GoToCellDialog::getColumn()
{
	return ui->columnEdit->text().toInt() - 1;
}

int GoToCellDialog::getRow()
{
	return ui->rowEdit->text().toInt() - 1;
}
