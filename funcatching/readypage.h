#ifndef READYPAGE_H
#define READYPAGE_H

#include <QWidget>
#include <mainwindow.h>

namespace Ui {
class readyPage;
}

class ReadyPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReadyPage(MainWindow *parent = 0);
    ~ReadyPage();

private slots:
    void on_back_clicked();

private:
    Ui::readyPage *ui;
    MainWindow *mainwindow;
};

#endif // READYPAGE_H
