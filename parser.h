#ifndef PARSER
#define PARSER

char* identifyFile();
int identifyLevel(int numLevels);
char** readLines(const char* fileName, int* lineCount);
void parseCompressedBoardData(const char *raw, Board board);

#endif  // PARSER
