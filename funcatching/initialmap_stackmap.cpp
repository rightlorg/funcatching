#include "initialmap_stackmap.h"
#include "ui_initialmap_stackmap.h"

initialmap_stackmap::initialmap_stackmap(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::initialmap_stackmap)
{
	ui->setupUi(this);
	//    position_checked = false;
	directory_exist = false;

	QRegExp regExp("[1-6][0-9]{0,7}");
	ui->columnEdit->setValidator(new QRegExpValidator(regExp, this));
	ui->rowEdit->setValidator(new QRegExpValidator(regExp, this));
	ui->columnEdit->setEnabled(true);
	ui->rowEdit->setEnabled(true);

	//    ui->paceEdit->setValidator(new QRegExpValidator(QRegExp("[0-2][0-9]{0,1}"),this));
}


initialmap_stackmap::~initialmap_stackmap()
{
	delete ui;
}

QString initialmap_stackmap::getFileName()
{
	return opendMapFileName;
}

void initialmap_stackmap::on_directoryButton_clicked()
{
	opendMapFileName = QFileDialog::getOpenFileName(this,tr("choose the edit map"),".",tr("map(*.map)"));
	QFile file(opendMapFileName);

	if(!file.open(QIODevice::ReadOnly)){
		QMessageBox::warning(this,tr("Map editor"),
				     tr("failed to read file %1:\n%2")
				     .arg(file.fileName())
				     .arg(file.errorString()));
		return;
	}
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_8);

	quint32 magic;
	in>>magic;

	if(magic!=map_MagicNum){
		QMessageBox::warning(this,tr("Map editor"),
				     tr("This file is mot a Map file\nPlease rechoose the edited file"));
		return;
	}
	ui->directoryLabel->setText(opendMapFileName);
	directory_exist = true;
}

//void initialmap_stackmap::on_poscheckBox_clicked()
//{
//    if(!directory_exist){
//        ui->poscheckBox->setChecked(false);
//        return;
//    }
//    position_checked = !position_checked;
//    ui->columnEdit->setEnabled(position_checked);
//    ui->rowEdit->setEnabled(position_checked);
//}
