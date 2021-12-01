/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsInit.h
*
* Description: Main driver for file system assignment.
*
* This file is the header file for fsInit which contains our
* struct for the vcb, some global variables, and functions for 
* initialization
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "fsEntry.h"
#include "fsFree.h"
#include "fsRoot.h"

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