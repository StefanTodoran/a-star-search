#include "logic.h"
#include <stdio.h>
#include <stdlib.h>

int countInstancesInBoard(Board board, enum TileType target) {
    int count = 0;

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j].id == target) {
                count++;
            }
        }
    }

    return count;
}

int getSpawnPosition(Board board, Position *position) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j].id == SPAWN) {
                position->y = i;
                position->x = j;
                return 1;
            }
        }
    }

    return 0;
}

void initializeGame(Board board, GameState *game) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            game->board[i][j] = board[i][j];
        }
    }

    Position position;
    getSpawnPosition(board, &position);
    board[position.y][position.x] = createBoardTile(EMPTY);
    game->player = position;
    
    const int maxCoins = countInstancesInBoard(board, COIN);
    game->maxCoins = maxCoins;

    game->coins = 0;
    game->keys = 0;
    game->won = false;
}

void copyGameState(GameState *original, GameState *copy) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            copy->board[i][j] = original->board[i][j];
        }
    }

    copy->player = original->player;
    copy->maxCoins = original->maxCoins;
    copy->coins = original->coins;
    copy->keys = original->keys;
    copy->won = original->won;
}

bool validTile(int y, int x) {
  return (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH);
}

struct BoardTile boundTileAt(Board board, int y, int x) {
  if (validTile(y, x)) {
    return board[y][x];
  }
  return createBoardTile(OUTSIDE);
}

bool tileTypeIncluded(enum TileType type, enum TileType validValues[], size_t numValidValues) {
    for (size_t i = 0; i < numValidValues; i++) {
        if (type == validValues[i]) {
            return true;
        }
    }
    return false;
}

bool canWalkOneWay(enum Direction direction, struct BoardTile tile) {
    if (direction == UP && tile.orientation == DOWN) return false;
    if (direction == DOWN && tile.orientation == UP) return false;
    if (direction == LEFT && tile.orientation == RIGHT) return false;
    if (direction == RIGHT && tile.orientation == LEFT) return false;

    return true;
}

bool canWalkTile(GameState *next, int y, int x, enum Direction direction) {
    if (!validTile(y, x)) {
        return false; // If it is outside the board, the player can't walk there.
    }

    struct BoardTile targetTile = next->board[y][x];
    if (targetTile.id == FLAG && next->coins == next->maxCoins) {
        return true; // We can only walk on the flag if we have all the coins!
    }

    enum TileType walkable[] = { EMPTY, SPAWN, ONEWAY };
    // if (extra) walkable = walkable.concat(extra);
    size_t numWalkables = sizeof(walkable) / sizeof(walkable[0]);

    bool canWalk = tileTypeIncluded(targetTile.id, walkable, numWalkables);
    if (targetTile.id == ONEWAY && !canWalkOneWay(direction, targetTile)) {
        printf("bad oneway");
        return false;
    }

    return canWalk;
}

bool attemptMove(GameState* next, int y, int x, enum Direction direction) {
    if (canWalkTile(next, y, x, direction)) { // extra: [ONEWAY]
        next->player.x = x;
        next->player.y = y;
        return true;
    }
    return false;
}

GameState* doGameMove(GameState *game, enum Direction move) {
    // First we create the next game state object following this game move.
    GameState* next = (GameState*)malloc(sizeof(GameState));
    copyGameState(game, next);

    Position moveTo; // Where the player is attempting to move.
    moveTo.y = game->player.y;
    moveTo.x = game->player.x;
    
    Position oneFurther; // One tile further that that in the same direction is.
    oneFurther.y = game->player.y;
    oneFurther.x = game->player.x;

    if (move == UP) {
        moveTo.y -= 1;
        oneFurther.y -= 2;
    } else if (move == DOWN) {
        moveTo.y += 1;
        oneFurther.y += 2;
    } else if (move == LEFT) {
        moveTo.x -= 1;
        oneFurther.x -= 2;
    } else if (move == RIGHT) {
        moveTo.x += 1;
        oneFurther.x += 2;
    }

    if (!validTile(moveTo.y, moveTo.x)) {
        // The user attempted to move outside the board.
        return game;
    }

    // Clear explosion tiles.
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (
                next->board[i][j].id == EXPLOSION ||
                next->board[i][j].id == LITTLE_EXPLOSION
            ) {
                next->board[i][j] = emptyTile;
            }
        }
    }

    // The basic structure of how this section works is that if the moveTo position is
    // on some tile that could be walked on after some game logic (e.g. a coin tile or 
    // door when keys > 0) then we do that logic and clear the tile. At the end of all the
    // logic, we run attemptMove which only succeeds and moves the player if moveTo is now
    // empty (e.g. the coin tile was collected & cleared or the door was opened & cleared)

    struct BoardTile moveToTile = next->board[moveTo.y][moveTo.x];
    struct BoardTile oneFurtherTile = boundTileAt(next->board, oneFurther.y, oneFurther.x);

    // If we walked onto a collectable, add it to the inventory
    // and clear the tile on the new board object.
    if (moveToTile.id == COIN) {
        next->coins += 1;
        next->board[moveTo.y][moveTo.x] = emptyTile;
    }
    if (moveToTile.id == KEY) {
        next->keys += 1;
        next->board[moveTo.y][moveTo.x] = emptyTile;
    }

    // If we walked into a door and have the means to open it, do so.
    if (game->keys > 0 && moveToTile.id == DOOR) {
        next->keys -= 1;
        next->board[moveTo.y][moveTo.x] = emptyTile;
    }

    // Pushing a crate onto an empty tile.
    if (moveToTile.id == CRATE && oneFurtherTile.id == EMPTY) {
        next->board[moveTo.y][moveTo.x] = emptyTile;
        next->board[oneFurther.y][oneFurther.x] = createBoardTile(CRATE);
    }

    // Pushing a crate into a crater.
    if (moveToTile.id == CRATE && oneFurtherTile.id == CRATER) {
        next->board[moveTo.y][moveTo.x] = emptyTile;
        next->board[oneFurther.y][oneFurther.x] = emptyTile;
    }

    // Pushing a bomb onto an empty tile.
    if (moveToTile.id == BOMB && oneFurtherTile.id == EMPTY) {
        next->board[moveTo.y][moveTo.x] = emptyTile;
        next->board[oneFurther.y][oneFurther.x] = moveToTile;
    }

    bool moved = attemptMove(next, moveTo.y, moveTo.x, move);
    return next;

    // if (moved) {
    //     // Tile entity logic handling. If we haven't moved, we shouldn't
    //     // decrease bomb fuse (invalid moves shouldn't count as a timestep).

    //     for (let i = 0; i < BOARD_HEIGHT; i++) {
    //         for (let j = 0; j < BOARD_WIDTH; j++) {
    //             const tile = next->board[i][j];

    //             if (tile.id == TileType.BOMB) {
    //                 tile.fuse--;

    //                 if (tile.fuse == 0) {
    //                     const littleExplosion: SimpleTile = { id: TileType.LITTLE_EXPLOSION };
    //                     if (boundTileAt(i - 1, j, next->board).id == TileType.CRATE) { next->board[i - 1][j] = littleExplosion; }
    //                     if (boundTileAt(i + 1, j, next->board).id == TileType.CRATE) { next->board[i + 1][j] = littleExplosion; }
    //                     if (boundTileAt(i, j - 1, next->board).id == TileType.CRATE) { next->board[i][j - 1] = littleExplosion; }
    //                     if (boundTileAt(i, j + 1, next->board).id == TileType.CRATE) { next->board[i][j + 1] = littleExplosion; }

    //                     next->board[i][j] = { id: TileType.EXPLOSION };
    //                 }
    //             }
    //         }
    //     }
    // }

    // next->won = winCondition(next);
    // return next;
}