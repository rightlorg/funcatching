#ifndef SERVER_START_H
#define SERVER_START_H

#include <QMainWindow>

namespace Ui {
class server_start;
}

class server_start : public QMainWindow
{
    Q_OBJECT

public:
    explicit server_start(QWidget *parent = 0);
    ~server_start();

private slots:
    void on_startorstopButton_clicked();

private:
    Ui::server_start *ui;
};

#endif // SERVER_START_H
