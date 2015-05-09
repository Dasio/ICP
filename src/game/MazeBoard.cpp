/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */


#include <iostream>

#include <algorithm>
#include <random>
#include <ctime>

#include "MazeBoard.h"


/**
  * Gets the minimum of three <code>int</code> values.
  *
  * @param a  value 1
  * @param b  value 2
  * @param c  value 3
  * @return  the smallest of the values
  */
static inline StoneType minimum(std::vector<int> &ratio)
{
    if (ratio[I] <= ratio[L] && ratio[I] <= ratio[T])
    {
        return I;
    }
    else if (ratio[L] <= ratio[I] && ratio[L] <= ratio[T])
    {
        return L;
    }
    return T;
}

static inline bool is_corner(int x, int y, int board_size)
{
    if ((x == 1 && y == 1) || (x == board_size && y == board_size))
        return true;
    else if ((x == 1 && y == board_size) || (x == board_size && y == 1))
        return true;
    return false;
}


void MazeBoard::initialize(int _board_size, int treasure_count, std::vector<Coords> &players_positions)
{
    board_size = _board_size; // adding board_size to class variable

    /* initialize random seed: */
    srand(time(NULL));

    std::vector<int> ratio (3, 0); // ratio between counts of Stone types
    board.resize(board_size * board_size);

    // adding corners to the board (4x L)
    board[INDEX(1,          1         )] = Stone(L, 1);
    board[INDEX(1,          board_size)] = Stone(L, 2);
    board[INDEX(board_size, 1         )] = Stone(L, 0);
    board[INDEX(board_size, board_size)] = Stone(L, 3);
    ratio[L] += 4;

    // adding T stones (odd x and odd y coordinates - but not corners)
    // count is ((N/2+1)*(N/2+1)-4)
    for (int x = 1; x <= board_size; x+=2)
    {
        for (int y = 1; y <= board_size; y+=2)
        {
            if (is_corner(x, y, board_size)) continue;
            board[INDEX(x,y)] = Stone(T, rand()%4);
            if      (x == 1)          (board[INDEX(x,y)]).rotation = 0; // first row
            else if (x == board_size) (board[INDEX(x,y)]).rotation = 2; // last row
            else if (y == 1)          (board[INDEX(x,y)]).rotation = 3; // first column
            else if (y == board_size) (board[INDEX(x,y)]).rotation = 1; // last column
            (ratio[T])++;
            std::cout << "ratio.. I: " << ratio[I] << " L: " << ratio[L] << " T: " << ratio[T] << std::endl;
        }
    }

    // placing remaining stones (with random shape and rotation)
    // it is trying to keep ratio 1:1:1
    int remaining_stones_cnt = (board_size/2)*board_size + (board_size/2+1)*(board_size/2);
    std::vector<Stone> temp_board (remaining_stones_cnt);
    StoneType min_ratio; // stone type with the smallest count of stones
    for (int x = 0; x < remaining_stones_cnt; x++)
    {
        min_ratio = minimum(ratio);
        temp_board[x].type = min_ratio;
        temp_board[x].rotation = rand() % 4;
        (ratio[min_ratio])++;
        //std::cout << "ratio.. I: " << ratio[I] << " L: " << ratio[L] << " T: " << ratio[T] << std::endl;
    }

    // shuffle remaining stones
    auto engine = std::default_random_engine{};
    std::shuffle(std::begin(board), std::end(board), engine);

    // adding remaining stones to the even rows (N/2)*N
    for (int x = 2; x <= board_size; x+=2)
    {
        for (int y = 1; y <= board_size; y++)
        {
            board[INDEX(x,y)] = temp_board.back();
            temp_board.pop_back();
        }
    }

    // adding remaining stones to the odd rows, where column is even ((N/2+1)*(N/2))
    for (int x = 1; x <= board_size; x+=2)
    {
        for (int y = 2; y <= board_size; y+=2)
        {
            board[INDEX(x,y)] = temp_board.back();
            temp_board.pop_back();
        }
    }

    addTreasures(treasure_count);
    free_stone.type = minimum(ratio);
}

void MazeBoard::addTreasures(int treasure_count)
{
    std::vector<int> temp_coords; //board.size() - 4

    for (int x = 1; x <= board_size; x++)
    {
        for (int y = 1; y <= board_size; y++)
        {
            if (is_corner(x,y, board_size))
                continue;
            temp_coords.push_back(INDEX(x,y));
        }
    }

    // shuffle coordinates where treasures will be placed in
    auto engine = std::default_random_engine{};
    std::shuffle(std::begin(temp_coords), std::end(temp_coords), engine);

    for (int x = 1; x <= treasure_count; x++)
    {
        board[temp_coords[x]].treasure = x;
    }
}


bool MazeBoard::shift(int x, int y)
{

    if (x == 1 && y%2 == 0) // shifting column down
    {

    }
    else if (x == 1 && y%2 == 0) // shifting column down
    {

    }
    else if (x == 1 && y%2 == 0) // shifting column down
    {

    }
    else if (x == 1 && y%2 == 0) // shifting column down
    {

    }
    else
        return false;


    return true;

    // add coordinates to forbidden_shift if board was shifted
}

void MazeBoard::rotateFreeStone()
{
    free_stone.rotation = (free_stone.rotation + 1) % 4;
}
