/**
 * driver.c by Brandon Morris
 * Please run this program on Ubuntu 16.04 LTS.
 */

#include <stdio.h>
#include <stdlib.h>
#include "directory.h"
#include "blockTable.h"

void startUp(long *sizePointer, long *blockPointer);
void getInput(BlockTable* table, Directory* directory);
void addFile(BlockTable* table, Directory* directory);
int checkForSpace(BlockTable* bTable, int fileSize);
void printSystem(BlockTable* table, Directory* directory);
void deleteFile(BlockTable* table, Directory* directory);
int main() {

    long* systemSizePtr = malloc(sizeof(long));
    long* blockSizePtr = malloc(sizeof(long));
    *systemSizePtr = 0;
    *blockSizePtr = 0;
    startUp(systemSizePtr, blockSizePtr);  // Get size values for the system from the user.

    BlockTable sysTable = createBlockTable(*blockSizePtr, (*systemSizePtr / *blockSizePtr));
    Directory sysDirectory = createDirectory(*systemSizePtr / *blockSizePtr);

    // Loop until the user enters the command to stop. Terminates the program inside the function.
    int loopFlag = 1;
    while (loopFlag > 0) {
        getInput(&sysTable, &sysDirectory);
    }

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
    char sizeInput[32];
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
    while (*blockPointer == 0 || *blockPointer < 0) {
        char* blockInput = malloc(sizeof(char) * 32);
        *blockPointer = 0;
        printf("Enter the size of each block: ");
        scanf("%s", blockInput);
        *blockPointer = strtol(blockInput, &endPointer, 10);
        long blockVal = *blockPointer;
        long sizeVal = *sizePointer;
        if (*blockPointer == 0 || *blockPointer < 0) {
            printf("Invalid input, please try again.\n");
            *blockPointer = -1;
        }
        else if (*blockPointer > *sizePointer) {
            printf("Not enough memory for a block of that size, please try again.\n");
            *blockPointer = -1;
        }
        else if (*sizePointer % *blockPointer != 0) {
            printf("TEST: %ld\n%ld\n", *blockPointer, *sizePointer);
            printf("Invalid size: Sum of all blocks must equal system size.\n");
            printf("Please enter a block size that the system size is perfectly divisible by.\n");
            *blockPointer = -1;
        }
         free(blockInput);
    }

}

/**
 * Gives the user a selection of actions to choose from, parses and validates their input, then
 * executes the command indicated by a valid input option.
 */
void getInput(BlockTable* bTable, Directory* directory) {
    char userInput[10];
    long inputValue = 0;
    char *endPointer;

    // Input validation.
    while (inputValue < 1 || inputValue > 4) {
        printf("Would you like to: \n");
        printf("Add a file? Enter 1\n");
        printf("Delete a file? Enter 2\n");
        printf("Print values? Enter 3\n");
        printf("Quit? Enter 4\n");
        scanf("%s", userInput);
        inputValue = strtol(userInput, &endPointer, 10);
        if (inputValue < 1 || inputValue > 4) {
            printf("Invalid selection, please try again.");
        }
    }

    // Launch the command corresponding to the user input.
    switch (inputValue) {
        case 1:
            addFile(bTable, directory);
            break;
        case 2:
            deleteFile(bTable, directory);
            break;
        case 3: // Print the contents of memory.
            printSystem(bTable, directory);
            break;
        case 4:
            printf("Exiting...");
            destroyDirectory(directory);
            destroyBlockTable(bTable);
            exit(0);
        default:
            printf("Something has gone wrong. Exiting...");
            destroyDirectory(directory);
            destroyBlockTable(bTable);
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
        printf("Not enough memory to add this file.\n\n");
    } else {  // There is enough space, add the file to the system.
        int blocksNeeded;
        if (fileSize % table->blockSize != 0) {
            blocksNeeded = (fileSize / table->blockSize) + 1;
        } else {
            blocksNeeded = (fileSize / table->blockSize);
        }
        printf("Blocks needed: %d", blocksNeeded);
        Entry newEntry = createEntry(fileName, fileSize, newFileIndex, blocksNeeded);
        updateTable(table, newFileIndex, fileSize);
        addToDirectory(directory, newEntry);
        printf("File added.\n\n");
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
    if (fileSize <= blockSize) {
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
        if ( (bTable->table + (sizeof(Block) * i))->inUse == 0 ) {    // Blocks have member field 'inUse' with arithmetic boolean values.
            blockCounter++;
            if (blockCounter == blocksNeeded) {  // Branch here if we found enough space for the file.
                newFileIndex = (i + 1) - blocksNeeded;
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

void deleteFile(BlockTable* table, Directory* directory) {
    char fileName[64];
    printf("Deleting - Enter the file name: ");
    scanf("%s", fileName);
    int blockLength, blockStart, i;
    int fileIndex = findEntryInDirectory(directory, fileName);
    if (fileIndex >= 0) {  // Branch here if an Entry matching the fileName was found.
        blockLength = (directory->list + (sizeof(Entry) * fileIndex))->length;
        blockStart = (directory->list + (sizeof(Entry) * fileIndex))->start;
        // Iterate over and reset BlockTable elements belonging to the deleted file.
        for (i = 0; i < blockLength; i++) {
            resetBlock((table->table + (sizeof(Block) * (i + blockStart))));
        }

        // Remove the Entry from the Directory.
        deleteFromDirectory(directory, fileIndex);

        printf("File successfully deleted.\n\n");
    } else {
        printf("No such file was found in the system.\n");
    }

}

void printSystem(BlockTable* table, Directory* directory) {
    printf("-------------------------------------------\n");
    printDirectory(directory);
    printf("-------------------------------------------\n");
    printTable(table);
    printf("-------------------------------------------\n\n");
}