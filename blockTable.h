//
// Created by Brandon Morris on 2019-10-31.
//

#ifndef BLOCK_H
#define BLOCK_H

typedef struct block Block;
typedef struct blockTable BlockTable;

/**
 * Definition of the Block type. Holds information about particular sections of system memory
 * and is used by the BlockTable.
 */
struct block {
    int size;
    int used;
    int fragmented;
    int inUse;
};

/**
 * Definition of the BlockTable type. Used to store individual Block objects.
 */
struct blockTable {
    int blockSize;
    int length;
    Block *table;
};

/* Block functions. */
Block createBlock(int size);
void updateBlock(Block block, int newUsed);
void resetBlock(Block block);
int isBlockInUse(Block block);

/* BlockTable functions. */
BlockTable createBlockTable(int blockSize, int length);
void destroyBlockTable(BlockTable* table);
void updateTable(BlockTable* bTable, int index, int sizeUsed);
void clearTable(BlockTable* bTable);
void printTable(BlockTable* bTable);

#endif
