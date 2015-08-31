#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QDialog>
#include "game.h"

namespace Ui {
class GameMenu;
}

class GameMenu : public QDialog
{
    Q_OBJECT
public:
    explicit GameMenu(QWidget *parent = 0, Game *gameP = 0);
    ~GameMenu();

private slots:
    void on_exitButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::GameMenu *ui;
    Game *game;
};

#endif // GAMEMENU_H
