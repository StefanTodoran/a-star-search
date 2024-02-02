#include <vector>
#include <stdlib.h>
#include "logic.h"
#include "search.h"
#include "heuristics.h"

int manhattanDistance(Position positionA, Position positionB) {
    return abs(positionA.x - positionB.x) + abs(positionA.y - positionB.y);
}

std::vector<Position> getTilePositions(const Board board, TileType type) {
    std::vector<Position> positions;
    const int dimensions[2] = {BOARD_HEIGHT, BOARD_WIDTH};

    for (int i = 0; i < dimensions[0]; i++) {
        for (int j = 0; j < dimensions[1]; j++) {
            if (board[i][j].id == type) {
                positions.push_back({i, j});
            }
        }
    }

    return positions;
}

int basicHeuristic(const SearchNode& searchNode) {
    const GameState& state = searchNode.game;

    std::vector<Position> tilesOfInterest = getTilePositions(state.board, FLAG);
    std::vector<Position> coinPositions = getTilePositions(state.board, COIN);
    tilesOfInterest.insert(tilesOfInterest.end(), coinPositions.begin(), coinPositions.end());

    int maxDistance = 0;
    for (const Position& tilePosition : tilesOfInterest) {
        int distance = manhattanDistance(state.player, tilePosition);
        maxDistance = std::max(maxDistance, distance);
    }

    return maxDistance;
}