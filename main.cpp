#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "util.h"
#include "logic.h"
#include "search.h"
#include "io.h"

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