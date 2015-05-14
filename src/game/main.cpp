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

void printCardsAndPlayers(Game &game)
{
    std::array<std::string,4> players;
    int N = game.labyrinth.getSize();
    Stone stone;
    for(int y=1; y<=N; y++)
    {
        for(int x=1; x<=N; x++)
        {
            stone = game.labyrinth.get(y,x);
            if(stone.treasure)
                std::cout << "Card " << stone.treasure << " on " << x << ":" << y << std::endl;
            for(int i =0;i<4;i++)
            {
                if (stone.player_slots[i] != nullptr)
                    players[i] = std::to_string(x) + ":" + std::to_string(y);
            }

        }
    }
    std::cout << std::endl;
    for(int i =0;i<4;i++)
    {
        if(!players[i].empty())
            std::cout << game.getPlayerName(i) << " is on " << players[i] << std::endl;
    }
}

int main(/*int argc, char *argv[]*/)
{
    std::string command;
    std::string name;
    Game game;
    int x,y;
    int size = 7;
    int treasures = 12;

    while(!std::cin.eof())
    {
        std::cin >> command;
        if(command == "exit")
        {
            return 0;
        }
        else if(command == "add")
        {
            std::cin >> name;
            game.addPlayer(name);
        }
        else if(command == "set")
        {
            std::cin >> command;
            if(command == "mapsize")
                std::cin >> size;
            else if(command == "treasures")
                std::cin >> treasures;
        }
        else if (command == "create")
        {
            if(!game.initialize(size,treasures))
                std::cout << "Failed to init game";
            else
                break;
        }
        else
        {
            std::cout << "Unknown command";
        }
    }
    while(!std::cin.eof())
    {
        drawMap(game);
        std::cout << std::endl;
        printCardsAndPlayers(game);
        std::cout << "Actual card: " << game.actualCard() << std::endl;
        std::cout << "On turn " << game.getPlayerName(game.activePlayer());
        if(game.nextAction() == MOVE)
            std::cout << "MOVE" << std::endl;
        else
            std::cout << " SHIFT" << std::endl;
        std::cout << std::endl;
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
