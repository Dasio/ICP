/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */



#ifndef MAZEBOARD_H
#define MAZEBOARD_H

#include <vector>
#include "Stone.h"


// get index to vector from board coordinates
#define INDEX(x,y) ((x-1) * board_size + (y-1))
#define INDEX_C(coord) (((coord).x-1) * board_size + ((coord).y-1))



class MazeBoard
{
public:
    void initialize(int board_size);
    bool addTreasures(int treasure_count);
    bool addPlayers(std::vector<Coords*> &player_positions);

    bool shift(int x, int y);
    void rotateFreeStone();

    inline Stone get(int x, int y) { return board[INDEX(x,y)]; }
    inline Stone getFreeStone() { return free_stone; }
    inline int getSize() { return board_size; }

    // check if position is on the board
    inline bool onBoard(Coords pos);

    // check if there is possible path from start_pos to end_pos
    bool checkPath(Coords start_pos, Coords end_pos, std::vector<Coords> &open_queue);

    // move player - returns true if treasure was collectet (USE CAREFULLY)
    bool movePlayer(Coords start_pos, Coords end_pos, int id, int treasure);


private:
    int board_size; // = N ... (map size is N*N)
    std::vector<Stone> board;

    Stone free_stone;
    Coords forbidden_shift; // for blocking reverse shift in the next turn
};



#endif // MAZEBOARD_H
