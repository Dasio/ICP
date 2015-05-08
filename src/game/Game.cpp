/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

#include <fstream>
#include <algorithm>
#include <random>

#include "Game.h"




bool Game::initialize(int board_size, int treasure_count)
{
    // verify if board_size is in {5, 7, 9, 11}
    if (board_size != 5 && board_size != 7 && board_size != 9 && board_size != 11)
        return false;

    // verify if treasure_count is in {12, 24} ... but not 24 if map_size will be 5x5
    if ((treasure_count != 12 && treasure_count != 24) || (treasure_count == 24 && board_size == 5))
        return false;

    // not enough players
    if (players.size() < 2)
        return false;

    // INIT card_pack with values and shuffle them
    for (int x = 1; x <= treasure_count; x++)
    {
        card_pack.push_back(x);
    }
    auto engine = std::default_random_engine{};
    std::shuffle(std::begin(card_pack), std::end(card_pack), engine);

    // initialize game_board
    labyrinth.initialize(board_size, treasure_count);

    // initialize players positions
    std::vector<Coords> corner_positions (4);
    corner_positions[0].x = 1;          corner_positions[0].y = 1;
    corner_positions[0].x = 1;          corner_positions[0].y = board_size;
    corner_positions[0].x = board_size; corner_positions[0].y = 1;
    corner_positions[0].x = board_size; corner_positions[0].y = board_size;
    std::shuffle(std::begin(corner_positions), std::end(corner_positions), engine);
    for (uint x = 0; x < players.size(); x++)
    {
        players[x].position = corner_positions[x];
        corner_positions.pop_back();
    }

    return true;
}


int Game::addPlayer(std::string name)
{
    if (players.size() < 4)
    {
        Player new_player{name};
        players.push_back(new_player);
        return players.size();
    }
    return -1;
}


inline void Game::nextPlayer()
{
    player_on_turn = (player_on_turn + 1) % players.size();
}


inline int Game::actualCard()
{
    return card_pack.back();
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// TODO
bool Game::saveGame(std::string file_name)
{
    std::ofstream outfile (file_name, std::ofstream::binary);

    return true;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// TODO
bool Game::loadGame(std::string file_name)
{
    std::ifstream outfile (file_name, std::ofstream::binary);

    return true;
}

