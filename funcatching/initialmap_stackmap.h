#ifndef INITIALMAP_STACKMAP_H
#define INITIALMAP_STACKMAP_H

#include <QWidget>
#include <QtGui>

namespace Ui {
class initialmap_stackmap;
}

class initialmap_stackmap : public QWidget
{
    Q_OBJECT

public:
    explicit initialmap_stackmap(QWidget *parent = 0);
    ~initialmap_stackmap();
    bool position_checked;
    bool directory_exist;

private slots:
    void on_directoryButton_clicked();

private:
    enum{MagicNum = 0x2FA3120C};
    Ui::initialmap_stackmap *ui;
};

#endif // INITIALMAP_STACKMAP_H
