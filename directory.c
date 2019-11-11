//
// Created by Brandon Morris on 2019-11-01.
//

#include "directory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
void destroyDirectory(Directory* d) {
    free(d->list);
}


/**
 * Adds a new Entry to the specified Directory.
 *
 * @param d The Directory to Entry will be added to.
 * @param e The Entry object to be added.
 */
void addToDirectory(Directory* d, Entry e) {
    if (d->size == d->length) {
        printf("Not enough space to add a new entry.");
    } else {
        *(d->list + (sizeof(Entry) * d->size)) = e;
        d->size++;
    }
}


/**
 * Takes a Directory and deletes the Entry at the specified index by shifting
 * following Entry memory locations down, overwriting the deleted index and preserving
 * Directory
 * @param d
 * @param index
 */
void deleteFromDirectory(Directory* d, int index) {
    int i, copyFlag, entrySize;
    copyFlag = 0;
    entrySize = sizeof(Entry);
    for (i = 0; i < d->length; i++) {
        if (i == index && copyFlag != 1) {
            copyFlag = 1;
        }
        // If the copyFlag is true, shift the next Entry size chunk of memory down.
        if (copyFlag == 1) {
            memcpy((d->list + (entrySize * i)), (d->list + (entrySize * (i + 1))), entrySize);
        }
        // If the next iteration will be the index we want to delete at, set the copy flag True.
        else if (i + 1 == index) {
            copyFlag = 1;
        }
    }

    // Deletion is done, update the Directory size.
    d->size--;
}


void printDirectory(Directory* d) {
    int i;
    printf("Directory table:\n");
    printf("Filename\t\t\t\t\t\tSize\t\tStart\t\tLength\n");
    for (i = 0; i < d->size; i++) {
        Entry e = *(d->list + (sizeof(Entry) * i));
        printf("%s\t\t\t\t\t\t%d\t\t\t%d\t\t\t%d\n", e.fileName, e.size, e.start, e.length);
    }
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
    e.fileName = malloc(sizeof(char) * strlen(fileName));
    strcpy(e.fileName, fileName);
    e.size = size;
    e.start = start;
    e.length = length;
    return e;
}


/**
 * Finds the index of a file within a Directory matching the desired file name. Used in conjunction
 * with deleteFromDirectory() to find desired files when attempting to remove them.
 *
 * @param directory The Directory object to be searched.
 * @param fileName The name of the file in an Entry object to be searched for.
 * @return The index of the Entry in the Directory object if found, -1 if the Entry is not found.
 */
int findEntryInDirectory(Directory* directory, char* fileName) {
    int i, result;
    result = -1;  // If no file matching the fileName given is found, return -1.
    for (i = 0; i < directory->size; i++) {
        if (strcmp( (directory->list + (sizeof(Entry) * i))->fileName, fileName) == 0) {
            result = i;  // Found the file, return the index of the file in the directory.
        }
    }
    return result;
}

