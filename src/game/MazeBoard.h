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



class MazeBoard
{
private:
    int board_size; // = N ... (map size is N*N)
    std::vector<Stone> board;


    Stone free_stone;
    Coords forbidden_shift; // for blocking reverse shift in the next turn

    void addTreasures(int treasure_count);

public:
    void initialize(int board_size, int treasure_count);
    bool shift(int x, int y);
    void rotateFreeStone();

    inline Stone get(int x, int y);
    inline Stone getFreeStone();
    inline int getSize();

};



#endif // MAZEBOARD_H
