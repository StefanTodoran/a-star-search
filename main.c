#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "logic.h"
#include "parser.h"

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

    while (true) {
        system("clear");
        printBoard(game.board, game.player);
        printInventory(&game);

        enum Direction move = promptPlayerMove();
        GameState* next = doGameMove(&game, move);
        game = *next;

        if (game.won) break;
    }

    // printf("\rValue of X is: %d", x/114);
    // fflush(stdout);

    return 0;
}