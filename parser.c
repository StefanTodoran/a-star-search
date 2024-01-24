#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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