//
// Created by Brandon Morris on 2019-11-01.
//

#ifndef DIRECTORY_H
#define DIRECTORY_H

typedef struct directory Directory;
typedef struct directory_entry Entry;

// TODO: Try putting these structs back into the .c file after testing.
struct directory {
    int length;
    int size;
    Entry* list;
};

struct directory_entry {
    char* fileName;
    int size;
    int start;
    int length;
} directory_entry;

Directory createDirectory(int length);
void destroyDirectory(Directory* d);
void addToDirectory(Directory* d, Entry e);
void deleteFromDirectory(Directory* d, int index);
Entry createEntry(char *fileName, int size, int start, int length);
int findEntryInDirectory(Directory* directory, char* fileName);
void printDirectory(Directory* d);

#endif
