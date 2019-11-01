//
// Created by Brandon Morris on 2019-11-01.
//

#ifndef DIRECTORY_H
#define DIRECTORY_H

typedef struct directory Directory;
typedef struct directory_entry Entry;

Directory createDirectory(int length);
void destroyDirectory(Directory d);
void addToDirectory(Directory d, Entry e);
void deleteFromDirectory(Directory d, int index);
Entry createEntry(char *fileName, int size, int start, int length);

#endif
