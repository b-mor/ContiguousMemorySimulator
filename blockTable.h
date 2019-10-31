//
// Created by Brandon Morris on 2019-10-31.
//

#ifndef BLOCK_H
#define BLOCK_H

typedef struct block Block;
typedef struct blockTable BlockTable;

/* Block functions. */
Block createBlock(int size);
void updateBlock(Block block, int newUsed);
void resetBlock(Block block);
int isBlockInUse(Block block);

/* BlockTable functions. */
BlockTable createBlockTable(int blockSize, int length);
void destroyBlockTable(BlockTable table);
void updateTable(BlockTable bTable, int index, int sizeUsed);
void clearTable(BlockTable bTable);

#endif
