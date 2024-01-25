#include "util.h"

const char* tileColors[] = {
    RESET, // EMPTY
    WALL, // WALL
    RESET GRN, // DOOR
    RESET GRN, // KEY
    RESET MAG, // CRATE
    RESET CYN, // CRATER
    RESET YEL, // COIN
    RESET, // SPAWN
    RESET YEL, // FLAG
    RESET RED, // BOMB
    RESET RED, // EXPLOSION
    RESET RED, // LITTLE_EXPLOSION
    RESET BLU, // ONEWAY
};