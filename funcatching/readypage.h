#ifndef READYPAGE_H
#define READYPAGE_H

#include <QWidget>

namespace Ui {
class readyPage;
}

class ReadyPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReadyPage(QWidget *parent = 0);
    ~ReadyPage();

private:
    Ui::readyPage *ui;
};

#endif // READYPAGE_H
