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

GameState* doGameMove(GameState *game, enum Direction move) {
    // First we create the next game state object following this game move.
    GameState* next = (GameState*)malloc(sizeof(GameState));
    copyGameState(game, next);

    Position moveTo; // Where the player is attempting to move.
    moveTo.y = game->player.y;
    moveTo.x = game->player.x;
    
    Position oneFurther; // One tile further that that in the same direction is.
    moveTo.y = game->player.y;
    moveTo.x = game->player.x;

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

    // TODO: Remove this code!

    next->player = moveTo;
    return next;

    // if (!validTile(moveTo.y, moveTo.x, next.board)) {
    //     // The user attempted to move outside the board.
    //     return game;
    // }

    // // Clear explosion tiles.
    // const dimensions = [next.board.length, next.board[0].length];
    // for (let i = 0; i < dimensions[0]; i++) {
    //     for (let j = 0; j < dimensions[1]; j++) {
    //         if (
    //             tileAt(i, j, next.board).id === TileType.EXPLOSION ||
    //             tileAt(i, j, next.board).id === TileType.LITTLE_EXPLOSION
    //         ) {
    //             next.board[i][j] = emptyTile;
    //         }
    //     }
    // }

    // // The basic structure of how this section works is that if the moveTo position is
    // // on some tile that could be walked on after some game logic (e.g. a coin tile or 
    // // door when keys > 0) then we do that logic and clear the tile. At the end of all the
    // // logic, we run attemptMove which only succeeds and moves the player if moveTo is now
    // // empty (e.g. the coin tile was collected & cleared or the door was opened & cleared)

    // const moveToTile = tileAt(moveTo.y, moveTo.x, next.board);
    // const oneFurtherTile = boundTileAt(oneFurther.y, oneFurther.x, next.board);

    // // If we walked onto a collectable, add it to the inventory
    // // and clear the tile on the new board object.
    // if (moveToTile.id === TileType.COIN) {
    //     next.coins += 1;
    //     next.board[moveTo.y][moveTo.x] = emptyTile;
    // }
    // if (moveToTile.id === TileType.KEY) {
    //     next.keys += 1;
    //     next.board[moveTo.y][moveTo.x] = emptyTile;
    // }

    // // If we walked into a door and have the means to open it, do so.
    // if (game.keys > 0 && moveToTile.id === TileType.DOOR) {
    //     next.keys -= 1;
    //     next.board[moveTo.y][moveTo.x] = emptyTile;
    // }

    // // Pushing a crate onto an empty tile.
    // if (moveToTile.id === TileType.CRATE && oneFurtherTile.id === TileType.EMPTY) {
    //     next.board[moveTo.y][moveTo.x] = emptyTile;
    //     next.board[oneFurther.y][oneFurther.x] = crateTile;
    //     next.soundEvent = SoundEvent.PUSH;
    // }

    // // Pushing a crate into a crater.
    // if (moveToTile.id === TileType.CRATE && oneFurtherTile.id === TileType.CRATER) {
    //     next.board[moveTo.y][moveTo.x] = emptyTile;
    //     next.board[oneFurther.y][oneFurther.x] = emptyTile;
    //     next.soundEvent = SoundEvent.FILL;
    // }

    // // Pushing a bomb onto an empty tile.
    // if (moveToTile.id === TileType.BOMB && oneFurtherTile.id === TileType.EMPTY) {
    //     next.board[moveTo.y][moveTo.x] = emptyTile;
    //     next.board[oneFurther.y][oneFurther.x] = moveToTile;
    //     next.soundEvent = SoundEvent.PUSH;
    // }

    // const moved = attemptMove(moveTo.y, moveTo.x, next, move);
    // if (moved) {
    //     // Tile entity logic handling. If we haven't moved, we shouldn't
    //     // decrease bomb fuse (invalid moves shouldn't count as a timestep).

    //     for (let i = 0; i < dimensions[0]; i++) {
    //         for (let j = 0; j < dimensions[1]; j++) {
    //             const tile = tileAt(i, j, next.board);

    //             if (tile.id === TileType.BOMB) {
    //                 tile.fuse--;

    //                 if (tile.fuse === 0) {
    //                     const littleExplosion: SimpleTile = { id: TileType.LITTLE_EXPLOSION };
    //                     if (boundTileAt(i - 1, j, next.board).id === TileType.CRATE) { next.board[i - 1][j] = littleExplosion; }
    //                     if (boundTileAt(i + 1, j, next.board).id === TileType.CRATE) { next.board[i + 1][j] = littleExplosion; }
    //                     if (boundTileAt(i, j - 1, next.board).id === TileType.CRATE) { next.board[i][j - 1] = littleExplosion; }
    //                     if (boundTileAt(i, j + 1, next.board).id === TileType.CRATE) { next.board[i][j + 1] = littleExplosion; }

    //                     next.board[i][j] = { id: TileType.EXPLOSION };
    //                     next.soundEvent = SoundEvent.EXPLOSION;
    //                 }
    //             }
    //         }
    //     }
    // }

    // next.won = winCondition(next);
    // return next;
}