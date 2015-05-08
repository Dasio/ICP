/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

#include "Stone.h"


bool Stone::canGo(Direction dir)
{
    if (dir == RIGHT)
    {
        if (type == I && rotation % 2 == 1)
            return true;
        else if (type == L && rotation < 2)
            return true;
        else if (type == T && rotation != 1)
            return true;
    }
    else if (dir == DOWN)
    {
        if (type == I && rotation % 2 == 0)
            return true;
        else if (type == L && (rotation == 1 || rotation == 2))
            return true;
        else if (type == T && rotation != 2)
            return true;
    }
    else if (dir == LEFT)
    {
        if (type == I && rotation % 2 == 1)
            return true;
        else if (type == L && rotation > 1)
            return true;
        else if (type == T && rotation != 3)
            return true;
    }
    else // UP
    {
        if (type == I && rotation % 2 == 0)
            return true;
        else if (type == L && (rotation == 0 || rotation == 3))
            return true;
        else if (type == T && rotation != 0)
            return true;
    }

    return false;
}
