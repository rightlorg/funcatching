#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
	w.show();
	return app.exec();
}
