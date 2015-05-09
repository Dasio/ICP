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
public:
    std::string name;
    int score;
    Coords position;

    Player(const std::string &player_name): name(player_name),score(0) {}
};



#endif // PLAYER_H
