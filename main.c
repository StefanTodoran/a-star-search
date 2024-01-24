#include <stdio.h>
#include <string.h>

#define MAX_FILE_NAME_LENGTH 100
#define LEVELS_FILE "levels.txt"

int main(int argc, char *argv[]) {
    char fileName[MAX_FILE_NAME_LENGTH];

    printf("Enter the levels file path or enter for default (" LEVELS_FILE "): ");
    fgets(fileName, sizeof(fileName), stdin);

    if (fileName[strlen(fileName) - 1] == '\n') { // Remove newline character if present.
        fileName[strlen(fileName) - 1] = '\0';
    }

    if (fileName[0] == '\0') { // Use default if user pressed Enter.
        strcpy(fileName, LEVELS_FILE);
    }

    // Verify the existence of the file
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: File \"%s\" not found or could not be opened.\n", fileName);
        return 1; // Exit with an error code
    }

    // Loop over each line of the file
    char line[256]; // Adjust the buffer size as needed
    while (fgets(line, sizeof(line), file) != NULL) {
        // Process each line as needed (you can print it as an example)
        printf("%s", line);
    }

    // Close the file
    fclose(file);

    return 0; // Exit successfully
}