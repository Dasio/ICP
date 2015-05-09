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


typedef enum {MOVE, SHIFT} Action;

class Game
{
public:
    MazeBoard labyrinth;

    // initialize board, fill with stones, with treasures and randomize players positions
    // there must be minimum of 4 players in the game
    bool initialize(int board_size = 7, int treasure_count = 12);

    // create new player - return his number or -1 if there are already 4 players
    int addPlayer(std::string name);

    std::string getPlayerName(uint id);
    int getPlayerScore(uint id);
    inline int getMaxScore() { return max_score; }

    inline int getPlayersCount() { return players.size(); }

    // click to the gameboard and take an action if it is allowed
    bool clickBoard(int x, int y);

    inline std::string* checkWinner() { return winner; }

    // return int representing the treasure on the top of card_pack
    // returns 0 if pack is empty
    int actualCard();

    inline int activePlayer() { return player_on_turn; }
    inline Action nextAction() { return next_action; }

    // save or load game
    bool saveGame(std::string file_name);
    bool loadGame(std::string file_name);

private:
    // TODO: vector of DATA CONTAINERS (streams?) with game_state for UNDO

    std::vector<int> card_pack;
    std::vector<Player> players;

    int player_on_turn;
    Action next_action;
    int max_score;

    std::string *winner;

    void stateOfGame();

    // try to move player to the coordinates [x,y]
    bool tryMovePlayer(int x, int y);
};



#endif // GAME_H
