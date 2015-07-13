#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QPixmap>
#include <QCloseEvent>
#include <QObject>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game();
    ~Game();

public:
    void connectServer();
    void firstDataSubmit();

private:
};

#endif // GAME_H
