#include <stdio.h>
#include <stdlib.h>
#include "directory.h"
#include "blockTable.h"

void startUp(long *sizePointer, long *blockPointer);
void getInput();
void addFile(BlockTable* table, Directory* directory);
int checkForSpace(BlockTable* bTable, int fileSize);

int main() {

    Directory d = createDirectory(5);
    Entry e1 = createEntry("test.txt", 100, 100, 0);
    Entry e2 = createEntry("test.txt", 200, 200, 0);
    Entry e3 = createEntry("test.txt", 300, 300, 0);

    addToDirectory(&d, e1);
    addToDirectory(&d, e2);
    addToDirectory(&d, e3);
    printDirectory(&d);
    deleteFromDirectory(&d, 1);
    printDirectory(&d);

    destroyDirectory(&d);
    return 0;
}

/**
 * Prompts the user to enter the size for both the total memory of the system
 * and the size for each block within the system.
 *
 * @param sizePointer Reference to the size of total system memory as given by the user.
 * @param blockPointer Reference to the size of individual blocks in memory as given by the user.
 */
void startUp(long *sizePointer, long *blockPointer) {
    char sizeInput[20];
    char *endPointer;
    *sizePointer = 0;

    // Input validation for the size of the system memory.
    while (*sizePointer == 0 || *sizePointer < 0) {
        printf("Enter the size of your storage device: ");
        scanf("%s", sizeInput);
        *sizePointer = strtol(sizeInput, &endPointer, 10);
        if (*sizePointer == 0 || *sizePointer < 0) {
            printf("Invalid input, please try again.");
        }
    }

    // Input validation for the size of each blockTable in system memory.
    char blockInput[20];
    *sizePointer = 0;
    while (*blockPointer == 0 || *blockPointer < 0) {
        printf("Enter the size of each blockTable: ");
        scanf("%s", blockInput);
        *blockPointer = strtol(blockInput, &endPointer, 10);
        if (*blockPointer == 0 || *blockPointer < 0) {
            printf("Invalid input, please try again.");
        }
        else if (*blockPointer > *sizePointer) {
            printf("Not enough memory for a blockTable of that size, please try again.");
        }
        else if (*blockPointer % *sizePointer != 0) {
            printf("Invalid size: Sum of all blocks must equal system size.");
            printf("Please enter a blockTable size that the system size is perfectly divisible by.");
        }
    }

}

/**
 * Gives the user a selection of actions to choose from, parses and validates their input, then
 * executes the command indicated by a valid input option.
 */
void getInput() {
    char userInput[10];
    long inputValue = 0;
    char *endPointer;

    // Input validation.
    while (inputValue < 1 || inputValue > 4) {
        printf("Would you like to: ");
        printf("Add a file? Enter 1");
        printf("Delete a file? Enter 2");
        printf("Print values? Enter 3");
        printf("Quit? Enter 4");
        scanf("%s", userInput);
        inputValue = strtol(userInput, &endPointer, 10);
        if (inputValue < 1 || inputValue > 4) {
            printf("Invalid selection, please try again.");
        }
    }

    // Launch the command corresponding to the user input.
    switch (inputValue) {
        case 1: // Add a file to the system.

        case 2: // Delete a file from the system.

        case 3: // Print the contents of memory.

        case 4: // Quit the program.
            printf("Exiting...");
            exit(0);
        default:
            printf("Something has gone wrong. Exiting...");
            exit(1);
    }

}


void addFile(BlockTable* table, Directory* directory) {
    char fileName[64];
    long fileSize = 0;
    char fileSizeInput[32];
    char* endPointer;
    int newFileIndex;
    // Start prompt for user input.
    printf("Adding - enter file name: ");
    scanf("%s", fileName);
    while (fileSize <= 0) {  // Input validation for file size.
        printf("\nAdding - enter file size: ");
        scanf("%s", fileSizeInput);
        fileSize = strtol(fileSizeInput, &endPointer, 10);
        if (fileSize <= 0) {
            printf("\nInvalid file size.");
        }
    }
    // Received values from user, add file to the system.
    newFileIndex = checkForSpace(table, fileSize);
    if (newFileIndex < 0) {  // Not enough space for the new file.
        printf("Not enough memory to add this file.");
    } else {  // There is enough space, add the file to the system.
        // TODO: Add functionality to updateTable so it can be called once and update multiple blocks.
        // TODO: Add file to directory, add file to block table.
    }
}


/**
 * Helper method for addFile. Checks the contents of the BlockTable to see if there is enough
 * contiguous memory available to add the new file. Will return the index of the first open block
 * with enough space to store the entire file if possible, if there is not enough space in memory
 * for the new file it will return -1.
 *
 * @param bTable The BlockTable to be checked for available space.
 * @param fileSize The size of the file attempting to be added.
 * @return The index of where the file should be stored if space is available, or -1 if there is none available.
 */
int checkForSpace(BlockTable* bTable, int fileSize) {
    int i, result, blockSize, newFileIndex, blocksNeeded, blockCounter, foundSpaceFlag;
    blockSize = bTable->blockSize;

    // Determine how many blocks of memory the new file will need.
    if (fileSize < blockSize) {
        blocksNeeded = 1;
    } else if (fileSize % blockSize == 0) {
        blocksNeeded = fileSize / blockSize;
    } else {
        blocksNeeded = (fileSize / blockSize) + 1;
    }

    // Iterate through the table and see if there is contiguous space for the new file.
    // If available space is available for the file, find the index of the first block where
    // we can store it.
    blockCounter = 0;
    foundSpaceFlag = 0;
    newFileIndex = -1;
    for (i = 0; i < bTable->length; i++) {
        if ( (bTable->table)->inUse == 0 ) {    // Blocks have member field 'inUse' with arithmetic boolean values.
            blockCounter++;
            if (blockCounter == blocksNeeded) {  // Branch here if we found enough space for the file.
                newFileIndex = i - blocksNeeded;
                foundSpaceFlag = 1;  // Set flag to binary true.
                break;
            }
        } else {    // Branch here if the block checked was already in use, reset the counter and continue search.
            blockCounter = 0;
        }
    }

    // If we found contiguous memory, return index of first open space. If not, return -1.
    if (foundSpaceFlag == 1) {
        result = newFileIndex;
    } else {
        result = -1;
    }

    return result;
}