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