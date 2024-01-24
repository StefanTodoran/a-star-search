#include "parser.h"

int main(int argc, char *argv[]) {
    const char* fileName = identifyFile();

    int lineCount;
    char** rawLines = readLines(fileName, &lineCount);

    // // TODO: replace
    // for (int i = 0; i < lineCount; i++) {
    //     // printf("Line %d: %s", i + 1, lines[i]);
    //     free(lines[i]);  // Free memory for each line.
    // }

    // // Free memory for the array of strings.
    // free(lines);

    return 0;
}