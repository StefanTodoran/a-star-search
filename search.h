#ifndef SEARCH
#define SEARCH

#include <vector>

typedef struct {
    GameState game;
    std::vector<Direction> path;
} SearchNode;

std::vector<Direction> aStarSearch(GameState start);

#endif  // SEARCH