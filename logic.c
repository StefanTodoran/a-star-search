#include <stdint.h>

enum TileType {
    EMPTY,
    WALL,
    DOOR,
    KEY,
    CRATE,
    CRATER,
    COIN,
    SPAWN,
    FLAG,
    BOMB,
    EXPLOSION,
    LITTLE_EXPLOSION,
    ONEWAY,
    OUTSIDE, // Used for out of bounds board queries.
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

struct OneWayTile {
    enum TileType id;
    enum Direction orientation;
};

struct BombTile {
    enum TileType id;
    int32_t fuse;
};

struct SimpleTile {
    enum TileType id;
};

union BoardTile {
    struct SimpleTile simpleTile;
    struct OneWayTile oneWayTile;
    struct BombTile bombTile;
};

#define createOneWayTile(orientation) ((struct OneWayTile){.id = ONEWAY, .orientation = (orientation)})
#define createBombTile(fuse) ((struct BombTile){.id = BOMB, .fuse = (fuse)})
#define createBoardTile(type) ((struct SimpleTile){.id = (type)})

typedef union BoardTile Board[14][8];