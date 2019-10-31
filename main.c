#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello, World!\n");
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
        case 1:
        case 2:
        case 3:
        case 4:
            printf("Exiting...");
            exit(0);
        default:
            printf("Something has gone wrong. Exiting...");
            exit(1);
    }

}