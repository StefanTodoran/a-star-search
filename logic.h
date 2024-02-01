#ifndef LOGIC
#define LOGIC

#include <stdbool.h>

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
        int fuse; // For BOMB tiles.
    };
};

#define emptyTile ((struct BoardTile){.id = EMPTY})
#define createBoardTile(type) ((struct BoardTile){.id = static_cast<enum TileType>(type)})
#define createBombTile(fuse) ((struct BoardTile){.id = BOMB, .fuse = (fuse)})
#define createOneWayTile(orientation) ((struct BoardTile){.id = ONEWAY, .orientation = static_cast<enum Direction>(orientation)})


#define BOARD_HEIGHT 14
#define BOARD_WIDTH 8
typedef struct BoardTile Board[BOARD_HEIGHT][BOARD_WIDTH];

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Board board;
    Position player;
    int maxCoins;
    int coins;
    int keys;
    bool won;
} GameState;

void initializeGame(Board board, GameState *game);
GameState* doGameMove(GameState *game, enum Direction move);

#endif  // LOGIC