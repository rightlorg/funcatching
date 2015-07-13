#ifndef GAME_H
#define GAME_H
#include <QtGui>
class Game
{
    Q_OBJECT
public:
    explicit Game();
    ~Game();

protected:
    void closeEvent(QCloseEvent *event);

public:
    void readHeadImage();
    void connectServer();
    void firstDataSubmit();

private:
    QString headImageFile;
    QPixmap *headImage;
};

#endif // GAME_H
