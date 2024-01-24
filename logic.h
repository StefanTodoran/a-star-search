#ifndef LOGIC
#define LOGIC

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

struct BoardTile {
    enum TileType id;
    union {
        enum Direction orientation; // For ONEWAY tiles.
        int32_t fuse; // For BOMB tiles.
    };
};

#define createBoardTile(type) ((struct BoardTile){.id = (type)})
#define createBombTile(fuse) ((struct BoardTile){.id = BOMB, .fuse = (fuse)})
#define createOneWayTile(orientation) ((struct BoardTile){.id = ONEWAY, .orientation = (orientation)})

#define BOARD_HEIGHT 14
#define BOARD_WIDTH 8
typedef struct BoardTile Board[BOARD_HEIGHT][BOARD_WIDTH];

#endif  // LOGIC