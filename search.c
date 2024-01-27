#include <stdlib.h>
#include "logic.h"

int manhattanDistance(Position positionA, Position positionB) {
    return abs(positionA.x - positionB.x) + abs(positionA.y - positionB.y);
}


bool areBoardStatesEqual(Board boardA, Board boardB) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (boardA[y][x].id != boardB[y][x].id) return false;

            switch (boardA[y][x].id) { // Handle additional properties conditioned on TileType.
                case BOMB:
                    if (boardA[y][x].fuse != boardB[y][x].fuse) return false;
                    break;
                case ONEWAY:
                    if (boardA[y][x].orientation != boardB[y][x].orientation) return false;
                    break;
            }
        }
    }

    return true;
}

bool areGameStatesEqual(GameState *stateA, GameState *stateB) {
    if (stateA->player.x != stateB->player.x) return false;
    if (stateA->player.y != stateB->player.y) return false;

    if (stateA->coins != stateB->coins) return false;
    if (stateA->keys != stateB->keys) return false;

    // if (stateA->won != stateB->won) return false; I don't think this needs to be checked.
    return areBoardStatesEqual(stateA->board, stateB->board);
}

typedef struct {
    GameState game;
    enum Direction direction;
} SuccessorState;

void getGameStateSuccessors(GameState gameState, SuccessorState successors[], int* numSuccessors) {
    enum Direction directions[] = {UP, RIGHT, DOWN, LEFT};
    *numSuccessors = 0;

    for (int i = 0; i < 4; i++) {
        GameState* successor = doGameMove(&gameState, directions[i]);

        if (!areGameStatesEqual(&gameState, successor)) {
            successors[*numSuccessors].game = *successor;
            successors[*numSuccessors].direction = directions[i];
            (*numSuccessors)++;
        }
    }
}

typedef int (*HeuristicFunction)(GameState *state);

void aStarSearch(GameState *start, HeuristicFunction heuristc) {
    //
}

// SuccessorState successors[4];
// int numSuccessors;
// getGameStateSuccessors
