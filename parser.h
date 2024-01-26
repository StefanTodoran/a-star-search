#ifndef PARSER
#define PARSER

char* identifyFile();
int identifyLevel(int numLevels);
enum Direction promptPlayerMove();

char** readLines(const char* fileName, int* lineCount);
void parseCompressedBoardData(const char *raw, Board board);

#endif  // PARSER
