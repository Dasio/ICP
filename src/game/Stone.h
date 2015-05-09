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
};

typedef enum {I, L, T} StoneType;
typedef enum {RIGHT, DOWN, LEFT, UP} Direction;


class Stone
{
public:
    StoneType type;  // type of the stone (I | L | T)
    int rotation;    // rotation clockwise (0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°)
    int treasure;

    bool canGo(Direction dir);

    Stone() = default;
    Stone(StoneType new_type, int rot=0) {type = new_type; rotation = rot;}
};



#endif // STONE_H
