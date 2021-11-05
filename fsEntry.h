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

typedef struct
{
    int blockLocation;
    char name[50];
    char type; // D = directory, F = file
    int size;
    // added :
    int blockCount; // counts number of logical blocks used
    int parent;     // need some way of tracking parent
} entryStruct, *entryStruct_p;