#ifndef INITIALMAP_STACKMAP_H
#define INITIALMAP_STACKMAP_H

#include <QWidget>

namespace Ui {
class initialmap_stackmap;
}

class initialmap_stackmap : public QWidget
{
    Q_OBJECT

public:
    explicit initialmap_stackmap(QWidget *parent = 0);
    ~initialmap_stackmap();

private:
    Ui::initialmap_stackmap *ui;
};

#endif // INITIALMAP_STACKMAP_H
