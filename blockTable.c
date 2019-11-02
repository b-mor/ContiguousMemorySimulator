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
void updateBlock(Block block, int newUsed) {
    block.used = newUsed;
    block.fragmented = block.size - newUsed;

    // Update inUse member field. If it is using space, make it true for inUse.
    // If the blockTable is now using none of its memory, reset it to default values.
    if (block.inUse == 0) {
        block.inUse = 1;
    } else if (block.inUse == 1 && block.used == 0) {
        resetBlock(block);
    }
}


/**
 * Resets a Block object to its default state, where used, fragmented, and inUse all equal zero.
 *
 * @param block The Block to be reset.
 */
void resetBlock(Block block) {
    block.used = 0;
    block.fragmented = 0;
    block.fragmented = 0;
}


/**
 * Checks whether or not a specific block is currently being used for system storage.
 *
 * @param block The Block being checked.
 * @return 0 (false) is Block is not in use, 1 (true) if it is.
 */
int isBlockInUse(Block block) {
    return block.inUse;

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
 *
 * @param bTable The BlockTable being updated.
 * @param index The index of the Block to be updated.
 * @param sizeUsed The new amount of space the Block will now be using.
 */
void updateTable(BlockTable* bTable, int index, int sizeUsed) {
    Block block = *(bTable->table + (sizeof(Block) * index));
    updateBlock(block, sizeUsed);

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
        resetBlock(*(bTable->table + (sizeof(Block) * i)));
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
