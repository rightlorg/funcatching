#ifndef STORING_PLAYER_H
#define STORING_PLAYER_H

class storing_player
{
public:
    storing_player();
    unsigned short x;
    unsigned short y;
    unsigned short z;
    bool identety;
    bool pos_changed;
private:
    bool operator ==(storing_player &next);
};

#endif // STORING_PLAYER_H
