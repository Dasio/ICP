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
    /**
     * @brief initialize
     * @param board_size
     */
    void initialize(int board_size);

    /**
     * @brief addTreasures
     * @param treasure_count
     * @return
     */
    bool addTreasures(int treasure_count);

    /**
     * @brief addPlayers
     * @param player_positions
     * @return
     */
    bool addPlayers(std::vector<Coords*> &player_positions);

    /**
     * @brief shift
     * @param x
     * @param y
     * @return
     */
    bool shift(int x, int y);

    /**
     * @brief rotateFreeStone by 90 clockwise
     */
    void rotateFreeStone();

    /**
     * @brief Get stone from board by [x,y]
     * @param x
     * @param y
     * @return Stone
     */
    inline Stone get(int x, int y) { return board[INDEX(x,y)]; }

    /**
     * @brief getFreeStone
     * @return Free stone
     */
    inline Stone getFreeStone() { return free_stone; }

    /**
     * @brief getSize
     * @return size
     */
    inline int getSize() { return board_size; }

    /**
     * @brief onBoard
     * @param pos
     * @return True if position is on the board
     */
    inline bool onBoard(Coords pos);

    /**
     * @brief checkPath
     * @param start_pos
     * @param end_pos
     * @param open_queue
     * @return True if there is possible path from start_pos to end_pos
     */
    bool checkPath(Coords start_pos, Coords end_pos, std::vector<Coords> &open_queue);


    /**
     * @brief movePlayer
     * @param start_pos
     * @param end_pos
     * @param id
     * @param treasure
     * @return true if treasure was collected
     */
    bool movePlayer(Coords start_pos, Coords end_pos, int id, int treasure);


private:
    int board_size; // = N ... (map size is N*N)
    std::vector<Stone> board;

    Stone free_stone;
    Coords forbidden_shift; // for blocking reverse shift in the next turn
};



#endif // MAZEBOARD_H
