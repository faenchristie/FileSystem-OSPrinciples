#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "fsEntry.h"

#define MAGICNUMBER 3141592654

/******************************************************************************
 * specifies information about how many blocks are there, what are the size of
 * each blocks, how many free blocks are there, etc.
 * uint64_t is an unsigned integer of 64 bits
 *****************************************************************************/
typedef struct {
	int blockSize;
	int blockCount;
	int rootDir; // location of rootdir starting block
	int rootDirBlocks; // amount of blocks rootDir takes up
	int freeSpaceMap;
	unsigned int signature;
} vcbStruct, *vcbStruct_p;

entryStruct *entry_p;

/******************************************************************************
 * GLOBAL VARIABLES HERE
 *****************************************************************************/
int blockLocation;
char name[50];
char type;
int size;
vcbStruct *vcb_p; // global variable for VCB
char *freeMap; // global variable for free space map to keep in memory

int findFreeBlocks(int blocksNeeded);

int initRootDir(uint64_t blockSize);

int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize);

void exitFileSystem();