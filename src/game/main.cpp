/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

#include <iostream>

#include "Game.h"

std::string stoneToString(Stone &stone)
{
    switch(stone.type)
    {
        case I:
            switch(stone.rotation)
            {
                case 0:
                case 2:
                    return "║";
                default:
                    return "═";
            }
        case L:
            switch(stone.rotation)
            {
                case 0:
                    return "╚";
                case 1:
                    return "╔";
                case 2:
                    return "╗";
                case 3:
                    return "╝";

            }
        case T:
            switch(stone.rotation)
            {
                case 0:
                    return "╦";
                case 1:
                    return "╣";
                case 2:
                    return "╩";
                case 3:
                    return "╠";
            }
    }
    return 0;
}
void drawMap(Game &game)
{
    int N = game.labyrinth.getSize();
    Stone stone;
    for(int y=1; y<=N; y++)
    {
        for(int x=1; x<=N; x++)
        {
            stone = game.labyrinth.get(y,x);
            std::cout << stoneToString(stone);

        }
        std::cout << std::endl;
    }
    stone = game.labyrinth.getFreeStone();
    std::cout << std::endl << "Free stone: " << stoneToString(stone) << std::endl;
}

int main(/*int argc, char *argv[]*/)
{
    std::string command;
    Game game;
    int x,y;
    game.addPlayer("Player1");
    game.addPlayer("Player2");

    if (!game.initialize())
        return 1;
    while(!std::cin.eof())
    {
        drawMap(game);
        std::cout << "On turn Player" << game.activePlayer() + 1;
        if(game.nextAction() == MOVE)
            std::cout << "MOVE" << std::endl;
        else
            std::cout << " SHIFT" << std::endl;
        std::cin >> command;
        if(command == "exit")
        {
            return 0;
        }
        else if(command == "rotate")
        {
            game.labyrinth.rotateFreeStone();
        }
        else if(command == "action")
        {
            std::cin >> x;
            std::cin >> y;
            std::cout << x << y << std::endl;
            if(!game.clickBoard(y,x))
                std::cout << "Failed" << std::endl;;
        }
        else
        {
            std::cout << "Unknown command";
        }
    }

}
