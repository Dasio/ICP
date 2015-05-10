/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */


//static const char *shape[] = {"I", "L", "T"};

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <fstream>

#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>

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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto engine = std::default_random_engine{seed};
    std::shuffle(std::begin(card_pack), std::end(card_pack), engine);

    // initialize players positions
    std::vector<Coords> corner_positions (4);
    std::vector<Coords*> player_positions; // for passing pointer to player positions to the board
    corner_positions[0].x = 1;          corner_positions[0].y = 1;
    corner_positions[1].x = 1;          corner_positions[1].y = board_size;
    corner_positions[2].x = board_size; corner_positions[2].y = 1;
    corner_positions[3].x = board_size; corner_positions[3].y = board_size;
    std::shuffle(std::begin(corner_positions), std::end(corner_positions), engine);
    for (uint x = 0; x < players.size(); x++)
    {
        players[x].position = corner_positions[x];
        //std::cout << "[" << players[x].position.x << "," << players[x].position.y << "]" << std::endl;
        player_positions.push_back(&(players[x].position));
    }

    // initialize game_board and insert players to the stones
    labyrinth.initialize(board_size);

    if (!labyrinth.addTreasures(treasure_count))
        return false;

    if (!labyrinth.addPlayers(player_positions))
        return false;

    next_action = SHIFT;
    player_on_turn = 0;
    max_score = treasure_count / players.size();
    winner = -1;


    std::stringstream ss;

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

std::string Game::getPlayerName(uint id)
{
    if (id < players.size())
        return players[id].name;
    return std::string();
}

int Game::getPlayerScore(uint id)
{
    if (id < players.size())
        return players[id].score;
    return -1;
}


//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// TODO
bool Game::clickBoard(int x, int y)
{
    // check if coordinates are on the board
    if (x <= 0 && x > labyrinth.getSize() && y <= 0 && y > labyrinth.getSize())
        return false;

    if (next_action == SHIFT)
    {
        if (labyrinth.shift(x, y))
        {
            next_action = MOVE;
            return true;
        }
    }
    else // next_action == MOVE
    {
        if (tryMovePlayer(x, y))
        {
            next_action = SHIFT;
            player_on_turn = (player_on_turn + 1) % players.size();
            return true;
        }
    }

    return false;
}


const std::string* Game::checkWinner()
{
    if (winner >= 0 && winner <= 4)
        return &(players[winner].name);
    return nullptr;
}



bool Game::tryMovePlayer(int x, int y)
{
    Player *act_player = &(players[player_on_turn]);
    std::vector<Coords> available_moves;

    if (labyrinth.checkPath(act_player->position, Coords(x,y), available_moves))
    {
        if (labyrinth.movePlayer(act_player->position, Coords(x,y), player_on_turn, actualCard()))
        {   //treasure was collected
            card_pack.pop_back();
            // add score and check winner if there isn't any already
            if (++(act_player->score) >= max_score && winner == -1)
                winner = player_on_turn;
        }
        return true;
    }
    return false;
}



int Game::actualCard()
{
    if (!card_pack.empty())
        return card_pack.back();
    else
        return 0;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// TODO
bool Game::saveGame(std::string file_name)
{
    std::ofstream ofs (file_name, std::ofstream::trunc);

    if (ofs.is_open())
    {
        saveState(ofs, XML);
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// TODO
bool Game::loadGame(std::string file_name)
{
    std::ifstream ifs (file_name/*, std::ofstream::binary*/);

    if (ifs.is_open())
    {
        // try and catch exceptions
        loadState(ifs, XML);
        return true;
        //////////////////////////// TODO check data ....
        //////////initialize undo history (clear stack)
        // place player positions to the stones on board if needed
    }

    return false;
}


void Game::saveState(std::ostream &stream, Format type)
{

    // make an archive (binary is not croos-platform portable)
    // xml for acces to state of saved game outside app
    switch (type)
    {
        case TXT:
        {
            boost::archive::text_oarchive toa(stream, boost::archive::no_header);
            toa << boost::serialization::make_nvp("Game", *this);
            break;
        }
        case BIN:
        {
            boost::archive::binary_oarchive boa(stream, boost::archive::no_header);
            boa << boost::serialization::make_nvp("Game", *this);
            break;
        }
        case XML:
        {
            boost::archive::xml_oarchive xoa(stream, boost::archive::no_header);
            xoa << boost::serialization::make_nvp("Game", *this);
            break;
        }
        default:
            break;
    }

}


void Game::loadState(std::istream &stream, Format type)
{
    switch (type)
    {
        case TXT:
        {
            boost::archive::text_iarchive tia(stream, boost::archive::no_header);
            tia >> boost::serialization::make_nvp("Game", *this);
            break;
        }
        case BIN:
        {
            boost::archive::binary_iarchive bia(stream, boost::archive::no_header);
            bia >> boost::serialization::make_nvp("Game", *this);
            break;
        }
        case XML:
        {
            boost::archive::xml_iarchive xia(stream, boost::archive::no_header);
            xia >> boost::serialization::make_nvp("Game", *this);
            break;
        }
        default:
            break;
    }
}


void Game::undo()
{

}


bool Game::canUndo()
{
    return false;
}
