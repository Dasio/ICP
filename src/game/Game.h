/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */

 /*

- pocet kamenov: N*N+1 (staticka premenna v Game)
- pocet kariet/predmetov: 12, 24 (impl 12) (staticka premenna v Game)

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

    // change player_on_turn to the next player
    inline void nextPlayer();

    bool movePlayer();

    // return int representing the treasure on the top of card_pack
    inline int actualCard();

    // save or load game
    bool saveGame(std::string file_name);
    bool loadGame(std::string file_name);

};



#endif // GAME_H
