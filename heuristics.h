#ifndef HEURISTICS
#define HEURISTICS

#include <vector>
#include "search.h"

// typedef int (*HeuristicFunction)(const GameState *state);

int basicHeuristic(const SearchNode& searchNode);

#endif  // HEURISTICS