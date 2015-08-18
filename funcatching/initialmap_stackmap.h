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
    Ui::initialmap_stackmap *ui;
private slots:
    void on_directoryButton_clicked();
    void on_poscheckBox_clicked();

private:
    enum{MagicNum = 0x2FA3120C};
    bool position_checked;
    bool directory_exist;

};

#endif // INITIALMAP_STACKMAP_H
