#include <cstdint>
#include <stdlib.h>
#include <set>
#include <queue>
#include <iostream>  // for std::cout
#include <ctime>     // for std::clock
#include <vector>
#include "logic.h"
#include "heuristics.h"
#include "search.h"

bool areBoardStatesEqual(const Board boardA, const Board boardB) {
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

bool areGameStatesEqual(const GameState *stateA, const GameState *stateB) {
    if (stateA->player.x != stateB->player.x) return false;
    if (stateA->player.y != stateB->player.y) return false;

    if (stateA->coins != stateB->coins) return false;
    if (stateA->keys != stateB->keys) return false;

    // if (stateA->won != stateB->won) return false; I don't think this needs to be checked.
    return areBoardStatesEqual(stateA->board, stateB->board);
}

uint64_t hashGameState(const GameState *state) { // TODO: Evaluate if this function has collisions.
    uint64_t hash = 17; // Initial value for hash.

    // Combine the hash values of individual components.
    hash = hash * 31 + ((uint64_t)state->player.x << 32 | state->player.y);
    hash = hash * 31 + state->coins;
    hash = hash * 31 + state->keys;

    // Combine the hash value of the board.
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            hash = hash * 31 + state->board[y][x].id;

            switch (state->board[y][x].id) {
                case BOMB:
                    hash = hash * 31 + state->board[y][x].fuse;
                    break;
                case ONEWAY:
                    hash = hash * 31 + state->board[y][x].orientation;
                    break;
            }
        }
    }

    return hash;
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

// struct CompareNodes {
//     HeuristicFunction heuristic;
//     CompareNodes(HeuristicFunction h) : heuristic(h) {}
//
//     bool operator()(const SearchNode& a, const SearchNode& b) const {
//         // Comparison function for the priority_queue
//         return heuristic(&a.game) > heuristic(&b.game);
//     }
// };

struct CompareNodes {
    bool operator()(const SearchNode& a, const SearchNode& b) const {
        return  basicHeuristic(a) > basicHeuristic(b);
    }
};

struct GameStateCompare {
    bool operator()(const GameState& a, const GameState& b) const {
        return areGameStatesEqual(&a, &b);
    }
};

// std::vector<Direction> aStarSearch(GameState start, HeuristicFunction heuristic) {
std::vector<Direction> aStarSearch(GameState start) {
    std::clock_t startTime = std::clock();
    std::cout << "\n\nStarting A* search...\n";

    SearchNode startState{start, {}};
    std::set<GameState, GameStateCompare> visited;

    int count = 0;
    // std::priority_queue<SearchNode, std::vector<SearchNode>, CompareNodes> frontier((CompareNodes(heuristic)));
    std::priority_queue<SearchNode, std::vector<SearchNode>, CompareNodes> frontier;
    frontier.push(startState);

    while (!frontier.empty()) {
        SearchNode currentState = frontier.top();
        frontier.pop();

        if (currentState.game.won) {
            std::clock_t endTime = std::clock();
            std::cout << "Searched " << count << " nodes.\n";
            std::cout << "Found a solution in " << static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC << " seconds:\n";
            for (const auto& step : currentState.path) {
                std::cout << static_cast<int>(step) << " ";
            }
            std::cout << std::endl;
            return currentState.path;
        }

        if (visited.find(currentState.game) != visited.end()) {
            continue;
        }

        visited.insert(currentState.game);
        int numSuccessors;
        SuccessorState successors[4];
        getGameStateSuccessors(currentState.game, successors, &numSuccessors);

        for (int i = 0; i < numSuccessors; ++i) {
            const auto& successor = successors[i];
            std::vector<Direction> path = currentState.path;
            path.push_back(successor.direction);

            SearchNode nextState{successor.game, path};
            frontier.push(nextState);
            count++;
        }
    }
}
