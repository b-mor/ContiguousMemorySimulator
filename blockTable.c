//
// Created by Brandon Morris on 2019-10-31.
//
#include "blockTable.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a new Block object.
 * All fields other than the size are initialized to zero, and shouldn't be modified while
 * the block is not in use in the system. Once the block is in use, the inUse field should be updated,
 * and the fragmented field should be the difference between used size and used.
 *
 * @param size of the new Block.
 * @return A Block object of with size = size, with other values initialized to 0.
 */
Block createBlock(int size) {
    Block newBlock;
    newBlock.size = size;
    newBlock.used = 0;
    newBlock.fragmented = 0;
    newBlock.inUse = 0;

    return newBlock;
}


/**
 * Updates a Block object to have new values based on the way the system is now using it's memory.
 *
 * @param block The Block object being modified.
 * @param newUsed The amount of space the Block object is now using.
 */
void updateBlock(Block* block, int newUsed) {
    block->used = newUsed;
    block->fragmented = block->size - newUsed;

    // Update inUse member field. If it is using space, make it true for inUse.
    // If the blockTable is now using none of its memory, reset it to default values.
    if (block->inUse == 0) {
        block->inUse = 1;
    } else if (block->inUse == 1 && block->used == 0) {
        resetBlock(block);
    }
}


/**
 * Resets a Block object to its default state, where used, fragmented, and inUse all equal zero.
 *
 * @param block The Block to be reset.
 */
void resetBlock(Block* block) {
    block->used = 0;
    block->fragmented = 0;
    block->fragmented = 0;
}


/**
 * Creates a new BlockTable object that uses a dynamically allocated array of Block objects
 * to represent the state of system memory. Only one BlockTable object should exist.
 *
 * @param blockSize Size of each block within the table.
 * @param length Length of the table
 * @return A BlockTable object that has a dynamically allocated array of Block objects, all set
 *         to their default values.
 */
BlockTable createBlockTable(int blockSize, int length) {
    BlockTable newTable;
    newTable.blockSize = blockSize;
    newTable.length = length;
    newTable.table = malloc(sizeof(struct blockTable) * length);

    int i;
    for (i = 0; i < length; i ++) {
        Block b = createBlock(blockSize);
        *(newTable.table + (sizeof(Block) * i)) = b;
    }

    return newTable;
}


/**
 * Frees all dynamically allocated memory from a BlockTable object.
 *
 * @param table The BlockTable object to be destroyed.
 */
void destroyBlockTable(BlockTable* bTable) {
    free(bTable->table);
}


/**
 * Updates the Block at the specified index within a BlockTable to use a new amount of memory.
 * If the size of the file is greater than one individual block, the update multiple blocks until the
 * BlockTable is fully updated, starting at the given index.
 *
 * NO BOUNDS CHECKING IS PERFORMED FOR THE UPDATE! It is the function caller's responsibility to check
 * for adequate space to store a file before calling this function. Space checking logic is handled by
 * checkForSpace() in the driver.c file before calling this function.
 *
 * @param bTable The BlockTable being updated.
 * @param index The index of the Block to be updated.
 * @param sizeUsed The new amount of space the Block will now be using.
 */
void updateTable(BlockTable* bTable, int index, int sizeUsed) {
    if (index > bTable->length || index < 0) {
        printf("Out of bounds: Block Table not updated.");
    } else {
        int blockSize = bTable->blockSize;
        if (sizeUsed < blockSize) {  // If size needed can fit in one block, just update that one.
            updateBlock((bTable->table + (sizeof(Block) * index)), sizeUsed);
        } else {  // If more than one block is needed to store the file, loop and update multiple blocks as needed.
            int i;
            int numBlocks = (sizeUsed / blockSize) + 1;  // Total number of blocks needed to store the file.
            for (i = 0; i < numBlocks; i++) {
                if (sizeUsed > blockSize) {  // Happens while we are filling blocks to max capacity
                    updateBlock((bTable->table + (sizeof(Block) * (index + i))), blockSize);
                    sizeUsed -= blockSize;
                } else {  // Last update of the loop, update the final block with the remainder memory.
                    updateBlock((bTable->table + (sizeof(Block) * (index + i))), sizeUsed);
                }
            }
        }
    }
}


/**
 * Resets the data for every Block within a BlockTable.
 *
 * @param bTable The BlockTable to have it's contents reset.
 */
void clearTable(BlockTable* bTable) {
    int i;
    // Reset the Block at every index of the BlockTable provided.
    for (i = 0; i < bTable->length; i++) {
        resetBlock((bTable->table + (sizeof(Block) * i)));
    }

}


/**
 * Prints the contents of the BlockTable to console.
 *
 * @param bTable The BlockTable to be printed.
 */
void printTable(BlockTable* bTable) {
    int i;
    printf("Block table:");
    printf("Block number\t\t\tSize used\t\t\tFragmented");
    for (i = 0; i < bTable->length; i++) {
        Block b = *(bTable->table + (sizeof(Block) * i));
        printf("%d\t\t\t%d\t\t\t%d\t\t\t%d", i, b.used, b.fragmented);
    }
}
