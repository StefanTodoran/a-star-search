#ifndef PARSER
#define PARSER

char* identifyFile();
int identifyLevel(int numLevels);
enum Direction promptPlayerMove();

char** readLines(const char* fileName, int* lineCount);
void parseCompressedBoardData(const char *raw, Board board);

void printBoard(const Board board, const Position player);
void printInventory(GameState *game);
void printPath(const std::vector<Direction>& directions);

#endif  // PARSER
