/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */



#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "Player.h"
#include "MazeBoard.h"


class Game
{
private:
    // TODO: vector of DATA CONTAINERS with game_state for UNDO

    std::vector<int> card_pack;
    std::vector<Player> players;

    int player_on_turn;

    void shuffleCards();

    void stateOfGame();


public:
    MazeBoard labyrinth;

    // initialize board, fill with stones, with treasures and randomize players positions
    bool initialize(int board_size = 7, int treasure_count = 12);

    // create new player - return his number or -1 if there are already 4 players
    int addPlayer(std::string name);

    std::string getPlayerName(uint id);
    int getPlayerScore(uint id);

    inline int getPlayersCount() { return players.size(); }

    // change player_on_turn to the next player
    void nextPlayer();

    bool movePlayer();

    // return int representing the treasure on the top of card_pack
    // returns 0 if pack is empty
    int actualCard();

    // save or load game
    bool saveGame(std::string file_name);
    bool loadGame(std::string file_name);

};



#endif // GAME_H
