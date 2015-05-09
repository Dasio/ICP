/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */


#include <iostream>

#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>

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


void MazeBoard::initialize(int _board_size)
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
            //std::cout << "ratio.. I: " << ratio[I] << " L: " << ratio[L] << " T: " << ratio[T] << std::endl;
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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto engine = std::default_random_engine{seed};
    std::shuffle(std::begin(temp_board), std::end(temp_board), engine);

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

    free_stone.type = minimum(ratio);
}

bool MazeBoard::addTreasures(int treasure_count)
{
    if (board_size == 0)
        return false;

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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto engine = std::default_random_engine{seed};
    std::shuffle(std::begin(temp_coords), std::end(temp_coords), engine);

    // fill first 'treasure_count' coordinates with treasures
    for (int x = 0; x < treasure_count; x++)
    {
        // treasures are numbered from 1
        board[temp_coords[x]].treasure = x + 1;
    }
    return true;
}


bool MazeBoard::addPlayers(std::vector<Coords*> &player_positions)
{
    if (board_size == 0)
        return false;

    for (uint x = 0; x < player_positions.size(); x++)
    {
        //std::cout << "generating player [" << (player_positions[x])->x << ","  << (player_positions[x])->y << "]" << std::endl;
        board[INDEX_C(*(player_positions[x]))].player_slots[x] = player_positions[x];
    }
    return true;
}


bool MazeBoard::shift(int clicked_x, int clicked_y)
{
    if (forbidden_shift.x == clicked_x && forbidden_shift.y == clicked_y)
        return false;

    Stone temp;

    if (clicked_x == 1 && clicked_y%2 == 0)
    {   // shifting even column down
        temp = board[INDEX(board_size, clicked_y)]; // store last elemnt in column
        // shift column down
        for (int x = board_size; x > 1; x--)
        {
            board[INDEX(x, clicked_y)] = board[INDEX(x-1, clicked_y)];
        }
        for (int x = 0; x < 4; x++) // move players from end of column
        {
            if (temp.player_slots[x])
            {
                free_stone.player_slots[x] = temp.player_slots[x];
                (free_stone.player_slots[x])->x = 1; // change players x position
                temp.player_slots[x] = nullptr;
            }
        }
        // add free stone to the start of column
        board[INDEX(1, clicked_y)] = free_stone;
        free_stone = temp;

        // prevent next move from reverse shift
        forbidden_shift.x = board_size; forbidden_shift.y = clicked_y;
    }
    else if (clicked_x == board_size && clicked_y%2 == 0)
    {   // shifting even column up
        temp = board[INDEX(1, clicked_y)]; // store first elemnt in column
        // shift column up
        for (int x = 1; x < board_size; x++)
        {
            board[INDEX(x, clicked_y)] = board[INDEX(x+1, clicked_y)];
        }
        for (int x = 0; x < 4; x++) // move players from end of column
        {
            if (temp.player_slots[x])
            {
                free_stone.player_slots[x] = temp.player_slots[x];
                (free_stone.player_slots[x])->x = board_size; // change players x position
                temp.player_slots[x] = nullptr;
            }
        }
        // add free stone to the end of column
        board[INDEX(board_size, clicked_y)] = free_stone;
        free_stone = temp;

        // prevent next move from reverse shift
        forbidden_shift.x = 1; forbidden_shift.y = clicked_y;
    }
    else if (clicked_x%2 == 0 && clicked_y == 1)
    {   // shifting even row right
        temp = board[INDEX(clicked_x, board_size)]; // store last elemnt in row
        // shift row right
        for (int y = board_size; y > 1; y--)
        {
            board[INDEX(clicked_x, y)] = board[INDEX(clicked_x, y-1)];
        }
        for (int x = 0; x < 4; x++) // move players from end of row
        {
            if (temp.player_slots[x])
            {
                free_stone.player_slots[x] = temp.player_slots[x];
                (free_stone.player_slots[x])->x = 1; // change players x position
                temp.player_slots[x] = nullptr;
            }
        }
        // add free stone to the start of row
        board[INDEX(clicked_x, 1)] = free_stone;
        free_stone = temp;

        // prevent next move from reverse shift
        forbidden_shift.x = clicked_x; forbidden_shift.y = board_size;
    }
    else if (clicked_x%2 == 0 && clicked_y == board_size)
    {   // shifting even row left
        temp = board[INDEX(clicked_x, 1)]; // store first elemnt in row
        // shift row left
        for (int y = 1; y < board_size; y++)
        {
            board[INDEX(clicked_x, y)] = board[INDEX(clicked_x, y+1)];
        }
        for (int x = 0; x < 4; x++) // move players from end of row
        {
            if (temp.player_slots[x])
            {
                free_stone.player_slots[x] = temp.player_slots[x];
                (free_stone.player_slots[x])->x = board_size; // change players x position
                temp.player_slots[x] = nullptr;
            }
        }
        // add free stone to the end of row
        board[INDEX(clicked_x, board_size)] = free_stone;
        free_stone = temp;

        // prevent next move from reverse shift
        forbidden_shift.x = clicked_x; forbidden_shift.y = 1;
    }
    else
        return false;

    return true;
}

void MazeBoard::rotateFreeStone()
{
    free_stone.rotation = (free_stone.rotation + 1) % 4;
}


bool MazeBoard::onBoard(Coords pos)
{
    if (pos.x > 0 && pos.x <= board_size && pos.y > 0 && pos.y <= board_size)
        return true;
    return false;
}



// typedef enum {RIGHT, DOWN, LEFT, UP} Direction;


// RECURSION
bool MazeBoard::checkPath(Coords start_pos, Coords end_pos)
{
    if (start_pos == end_pos)
        return true;

    // expand to right
    if (board[INDEX_C(start_pos)].canGo(RIGHT) && onBoard(start_pos.right())
        && board[INDEX_C(start_pos.right())].canGo(LEFT)
        && checkPath(start_pos.right(), end_pos))
            return true;
    else if (board[INDEX_C(start_pos)].canGo(DOWN) && onBoard(start_pos.down())
        && board[INDEX_C(start_pos.down())].canGo(UP)
        && checkPath(start_pos.down(), end_pos))
            return true;
    else if (board[INDEX_C(start_pos)].canGo(LEFT) && onBoard(start_pos.left())
        && board[INDEX_C(start_pos.left())].canGo(RIGHT)
        && checkPath(start_pos.left(), end_pos))
            return true;
    else if (board[INDEX_C(start_pos)].canGo(UP) && onBoard(start_pos.up())
        && board[INDEX_C(start_pos.up())].canGo(DOWN)
        && checkPath(start_pos.up(), end_pos))
            return true;

    return false;
}
