#include "logic.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void printBoard(const Board board) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            int tileId = board[i][j].id;

            // Determine padding based on the number of digits
            int padding = (tileId < 10) ? 2 : 1;

            // Print the ID with padding
            printf("%*d ", padding, tileId);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    const char* fileName = identifyFile();

    int lineCount;
    char** rawLines = readLines(fileName, &lineCount);

    for (int i = 0; i < lineCount; i++) {
        Board board;
        parseCompressedBoardData(rawLines[i], board);
        printBoard(board);
        free(rawLines[i]);  // Free memory for each line.
    }

    // Free memory for the array of strings.
    free(rawLines);

    return 0;
}