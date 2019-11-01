//
// Created by Brandon Morris on 2019-11-01.
//

#include "directory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
};


/**
 * Creates a new Directory object with space for 'length' number of Entry objects.
 * The size of each directory initializes to zero and serves as a counter for the
 * number of Entry objects referenced by this Directory object.
 *
 * @param length The number of Entry objects this Directory can contain.
 * @return A newly initialized Directory object.
 */
Directory createDirectory(int length) {
    Directory d;
    d.length = length;
    d.size = 0;
    d.list = malloc(sizeof(Entry) * length);
    return d;
}


/**
 * Frees the dynamically allocated memory associated with a Directory object.
 *
 * @param d The directory to have it's memory freed.
 */
void destroyDirectory(Directory d) {
    free(d.list);
}


/**
 * Adds a new Entry to the specified Directory.
 *
 * @param d The Directory to Entry will be added to.
 * @param e The Entry object to be added.
 */
void addToDirectory(Directory d, Entry e) {
    if (d.size == d.length) {
        printf("Not enough space to add a new entry.");
    } else {
        *(d.list + (sizeof(Entry) * d.size)) = e;
        d.size++;
    }
}


/**
 * Takes a Directory and deletes the Entry at the specified index by shifting
 * following Entry memory locations down, overwriting the deleted index and preserving
 * Directory
 * @param d
 * @param index
 */
void deleteFromDirectory(Directory d, int index) {
    int i, copyFlag, entrySize;
    copyFlag = 0;
    entrySize = sizeof(Entry);
    for (i = 0; i < d.length; i++) {
        // If the copyFlag is true, shift the next Entry size chunk of memory down.
        if (copyFlag == 1) {
            memcpy((d.list + (entrySize * i)), (d.list + (entrySize * (i + 1))), entrySize);
        }
        // If the next iteration will be the index we want to delete at, set the copy flag True.
        else if (i + 1 == index) {
            copyFlag = 1;
        }
    }

    // Deletion is done, update the Directory size.
    d.size--;
}


/**
 * Creates a new Entry object. Entry objects are used by the Directory object to
 * keep track of file names and indices where files begin and end in memory.
 *
 * @param fileName Name of the file this Entry refers to.
 * @param size Size the file holds in memory.
 * @param start The index in the BlockTable where memory for this file starts.
 * @param length The number of Blocks this file spans in memory.
 * @return A newly initialized Entry object.
 */
Entry createEntry(char *fileName, int size, int start, int length) {
    Entry e;
    e.fileName = fileName;
    e.size = size;
    e.start = start;
    e.length = length;
    return e;
}
