#ifndef INITIALIZEMAP_H
#define INITIALIZEMAP_H

#include <QMainWindow>

namespace Ui {
class initializemap;
}

class initializemap : public QMainWindow
{
    Q_OBJECT

public:
    explicit initializemap(QWidget *parent = 0);
    ~initializemap();

private slots:
    void on_posCheck_clicked();

    void on_cancelButton_clicked();

    void on_okButton_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_directoreButton_clicked();

private:
    Ui::initializemap *ui;
};

#endif // INITIALIZEMAP_H
