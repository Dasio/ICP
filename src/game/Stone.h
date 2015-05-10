/* Authors:
 * Dávid Mikuš      (xmikus15)
 * Peter Hostačný   (xhosta03)
 */


#ifndef STONE_H
#define STONE_H


class Coords
{
public:
    int x;
    int y;

    bool operator==(const Coords& other)
    {
        return (x == other.x && y == other.y);
    }
    bool operator!=(const Coords& other)
    {
        return (x != other.x || y != other.y);
    }
    Coords() = default;
    Coords(const int _x, const int _y) : x(_x), y(_y) {}

    inline Coords right() { return Coords(x, y+1); }
    inline Coords down() { return Coords(x+1, y); }
    inline Coords left() { return Coords(x, y-1); }
    inline Coords up() { return Coords(x-1, y); }
};

typedef enum {I, L, T} StoneType;
typedef enum {RIGHT, DOWN, LEFT, UP} Direction;


class Stone
{
public:
    StoneType type;  // type of the stone (I | L | T)
    int rotation;    // rotation clockwise (0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°)
    int treasure;

    Coords* player_slots[4]; // max. 4 players on the stone

    bool canGo(Direction dir);

    Stone() : type{I}, rotation{0}, treasure{0}, player_slots{0} {};
    Stone(const StoneType new_type, const int rot=0)
        : type{new_type}, rotation{rot}, treasure{0}, player_slots{0} {}
};



#endif // STONE_H
