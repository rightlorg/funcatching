#include "storing_player.h"

storing_player::storing_player()
{
    identety = false;
    pos_changed = false;
}

bool storing_player::operator ==(storing_player &next)
{
    if(this->z==next.z)
        if(this->x==next.x)
            if(this->y==next.y)
                return 1;
    return 0;
}
