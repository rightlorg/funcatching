#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

namespace Ui {
        class MainWindow;
}

class MainWindow : public QMainWindow
{
                Q_OBJECT
        public:
                explicit MainWindow(QWidget *parent = 0);
                ~MainWindow();

private slots:
    void on_startGame_clicked();
    void on_settings_clicked();

private:
    void createStatusBar();
    QLabel *statusLabel;
    QImage *statusImage = new QImage;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
