#ifndef UTIL
#define UTIL

#define NORMAL "\033[0m"
#define RESET  "\033[0m"
#define BOLD   "\033[1m"

#define RED "\033[31m"
#define GRN "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define MAG "\033[35m"
#define CYN "\033[36m"

const char* tileColors[] = {
    NORMAL,
    BOLD,
    GRN,
    GRN,
    MAG,
    CYN,
    YEL,
    "", // Tile of type SPAWN shouldn't be rendered ever.
    YEL,
    RED,
    RED,
    RED,
    BLU,
};

#endif  // UTIL