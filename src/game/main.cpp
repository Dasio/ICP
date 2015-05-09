/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

#include <iostream>

#include "Game.h"



int main(/*int argc, char *argv[]*/)
{
    Game game;

    game.addPlayer("Player1");
    game.addPlayer("Player2");

    if (game.initialize())
        std::cout << "Game" << std::endl;

}
