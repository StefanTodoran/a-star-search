#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "util.h"
#include "logic.h"
#include "parser.h"
#include "search.h"

void printBoard(const Board board, const Position player) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            struct BoardTile tile = board[i][j];
            int tileId = tile.id;

            if (i == player.y && j == player.x) {
                printf("%s %s ", RESET BOLD, "P");
            } else if (tileId == BOMB) {
                // Determine padding based on the fuse timer.
                int padding = (tile.fuse < 10) ? 2 : 1;
                printf("%s%*d ", tileColors[tileId], padding, tile.fuse);
            } else {
                // Determine padding based on the number of digits.
                int padding = (tileId < 10) ? 2 : 1;
                printf("%s%*d ", tileColors[tileId], padding, tileId);
            }
        }
        printf(RESET "\n");
    }
}

void printInventory(GameState *game) {
    printf("%d keys, %d/%d coins\n", game->keys, game->coins, game->maxCoins);
}

void printPath(const std::vector<Direction>& directions) {
    for (const auto& direction : directions) {
        switch (direction) {
            case UP:
                std::cout << "UP ";
                break;
            case RIGHT:
                std::cout << "RIGHT ";
                break;
            case DOWN:
                std::cout << "DOWN ";
                break;
            case LEFT:
                std::cout << "LEFT ";
                break;
            default:
                // Handle unexpected enum values if necessary
                std::cout << "UNKNOWN ";
                break;
        }
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    const char* fileName = identifyFile();

    int lineCount;
    char** rawLines = readLines(fileName, &lineCount);
    int targetLevel = identifyLevel(lineCount);

    Board board;
    parseCompressedBoardData(rawLines[targetLevel], board);
    
    GameState game;
    initializeGame(board, &game);

    for (int i = 0; i < lineCount; i++) {
        free(rawLines[i]);  // Free memory for each line.
    }
    // Free memory for the array of strings.
    free(rawLines);

    // Gameplay loop, used for testing purposes:
    // while (true) {
    //     system("clear");
    //     printBoard(game.board, game.player);
    //     printInventory(&game);

    //     enum Direction move = promptPlayerMove();
    //     GameState* next = doGameMove(&game, move);
    //     game = *next;

    //     if (game.won) break;
    // }

    printBoard(game.board, game.player);
    std::vector path = aStarSearch(game);
    // printPath(path);

    return 0;
}