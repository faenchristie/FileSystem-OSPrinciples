/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: fsEntry.h
*
* Description: Represents entries whether they be files
or directories.
**************************************************************/

#include <stdlib.h>
#include <unistd.h>

#define DIRENTRIES 50

typedef struct entry
{
    int blockLocation;
    char name[50];
    char type;
    int size;
} entry;