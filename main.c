#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "logic.h"
#include "parser.h"

void printBoard(const Board board, const Position player) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            int tileId = board[i][j].id;

            // Determine padding based on the number of digits.
            int padding = (tileId < 10) ? 2 : 1;

            if (i == player.y && j == player.x) { // Display the player with padding.
                printf("%s%*s ", RESET BOLD, padding, "P");
            } else { // Print the ID with padding.
                printf("%s%*d ", tileColors[tileId], padding, tileId);
            }
        }
        printf(RESET "\n");
    }
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
        enum Direction move = promptPlayerMove();
        GameState* next = doGameMove(&game, move);
        game = *next;
    }

    return 0;
}