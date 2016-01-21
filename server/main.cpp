#include <QApplication>
#include <QtCore>

#include <stdlib.h>

#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog dialog;
#ifdef Q_OS_SYMBIAN
    dialog.showMaximized();
#else
    dialog.show();
#endif
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return app.exec();
}
