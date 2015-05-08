/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */


#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "Stone.h"


class Player
{
private:
    std::string name;
    int score;

public:
    Coords position;

    inline std::string getName() {return name;};
    inline int getScore() {return score;};

    Player(std::string player_name)
    {
        name = player_name;
        score = 0;
    }
};



#endif // PLAYER_H
