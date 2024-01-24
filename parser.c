#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logic.h"

#define MAX_FILE_NAME_LENGTH 100
#define MAX_LINE_LENGTH 300 // TODO: May need to tweak this later.
#define LEVELS_FILE "levels.txt"

char* identifyFile() {
    char* fileName = (char*)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    if (fileName == NULL) {
        perror("Fatal memory allocation error.");
        exit(1);
    }

    promptInput:
    printf("Enter the levels file path or enter for default (" LEVELS_FILE "): ");
    fgets(fileName, MAX_FILE_NAME_LENGTH, stdin);

    if (fileName[strlen(fileName) - 1] == '\n') { // Remove newline character if present.
        fileName[strlen(fileName) - 1] = '\0';
    }

    if (fileName[0] == '\0') { // Use default if user pressed Enter.
        strcpy(fileName, LEVELS_FILE);
    }

    // Verify the existence of the file.
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: File \"%s\" not found or could not be opened. Please try again.\n", fileName);
        goto promptInput;
    }

    fclose(file);
    return fileName;
}

int identifyLevel(int numLevels) {
    int targetLevel;

    promptInput:
    printf("Enter the index of the level to select: ");
    int readStatus = scanf("%d", &targetLevel);

    if (readStatus != 1) {
        printf("Error: Invalid input, must enter a single integer.\n");
        while (getchar() != '\n');  // Keep reading characters to clear input buffer.
        goto promptInput;
    }

    if (targetLevel >= numLevels) {
        printf("Error: Invalid index. Level file only contains %d levels.\n", targetLevel);
        goto promptInput;
    }

    return targetLevel;
}

char** readLines(const char *fileName, int* lineCount) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Count the number of lines in the file.
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
    }

    // Reset the file pointer to the beginning of the file.
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the array of strings.
    char** lines = (char**)malloc(count * sizeof(char*));
    if (lines == NULL) {
        perror("Fatal memory allocation error.");
        exit(1);
    }

    // Read each line and store it in the array.
    for (int i = 0; i < count; i++) {
        lines[i] = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
        if (lines[i] == NULL) {
            perror("Fatal memory allocation error.");
            exit(1);
        }
        if (fgets(lines[i], MAX_LINE_LENGTH, file) == NULL) {
            perror("Error reading line from file.");
            exit(1);
        }
    }

    fclose(file);
    *lineCount = count;
    return lines;
}

#define ROW_DELIMITER "/"
#define COLUMN_DELIMITER ","
#define TILE_DELIMITER "."

void parseCompressedBoardData(const char *raw, Board board) {
    char *rawBoard = strdup(raw);
    char *freeRawBoard = rawBoard;
    char *strRow = strtok_r(rawBoard, ROW_DELIMITER, &rawBoard);

    int rowIndex = 0;
    while (strRow != NULL && rowIndex < BOARD_HEIGHT) {
        char *strTile = strtok_r(strRow, COLUMN_DELIMITER, &strRow);
        int columnIndex = 0;

        while (strTile != NULL && columnIndex < BOARD_WIDTH) {
            if (strstr(strTile, TILE_DELIMITER) != NULL) {
                char *token = strtok_r(strTile, TILE_DELIMITER, &strTile);

                if (token != NULL) {
                    int tileId = atoi(token);
                    struct BoardTile tile;

                    if (tileId == ONEWAY) {
                        token = strtok_r(NULL, TILE_DELIMITER, &strTile);
                        int orientation = atoi(token);
                        tile = createOneWayTile(orientation);
                    } 
                    if (tileId == BOMB) {
                        token = strtok_r(NULL, TILE_DELIMITER, &strTile);
                        int fuse = atoi(token);
                        tile = createBombTile(fuse);
                    }

                    board[rowIndex][columnIndex] = tile;
                }
            } else {
                int tileId = atoi(strTile);
                board[rowIndex][columnIndex] = createBoardTile(tileId);
            }

            strTile = strtok_r(NULL, COLUMN_DELIMITER, &strRow);
            columnIndex++;
        }

        strRow = strtok_r(NULL, ROW_DELIMITER, &rawBoard);
        rowIndex++;
    }

    free(freeRawBoard);
}