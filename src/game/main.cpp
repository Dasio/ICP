/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

#include <iostream>

#include "Game.h"



int main(/*int argc, char *argv[]*/)
{
    std::cout << "Game" << std::endl;

    Game game;

    game.addPlayer("Player1");
    game.addPlayer("Player2");

    game.initialize();

}
