/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsInit.c
*
* Description: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
* initialize the volume control block
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsEntry.h"
#include "fsDirectory.h"
#include "fsFree.h"

#define MAGICNUMBER 3141592654

/******************************************************************************
 * specifies information about how many blocks are there, what are the size of
 * each blocks, how many free blocks are there, etc.
 * uint64_t is an unsigned integer of 64 bits
 *****************************************************************************/
typedef struct
{
	int blockSize;
	int blockCount;
	int rootDir;
	int freeSpaceMap;
	unsigned int signature;
} vcbStruct, *vcbStruct_p;

/******************************************************************************
 * GLOBAL VARIABLES HERE
 *****************************************************************************/
int blockLocation;
char name[50];
char type;
int size;

// global variable for VCB
vcbStruct *vcb_p;

// global variable for free space map to keep in memory
char *freeMap;

// fix this
// we need to find how large this must be
// potentially malloc/realloc?
entryStruct *listOfEntries;
int numberOfEntries = 0;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	/**************************************************************************
	* --------------------Initializing the file system-------------------------
	* First, store a mallaoc of blocksize into the vcb pointer variable,
	* then calling an LBAread, with block 0. 
	* Next, check if the magic number number in the structure matches with the 
	* pointer. If they magic numbers do not match, the initialization will
	* occur for the VCB, freespace, and the root directory.
	**************************************************************************/
	printf("Initializing File System with %lld blocks with a block size of %lld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	printf("About to allocate %ld bytes for VCB\n", sizeof(vcbStruct));
	vcb_p = malloc(blockSize);

	LBAread(vcb_p, 1, 0);

	if (vcb_p->signature != MAGICNUMBER)
	{
		////////////// CHANGE LATER /////////////////
		// list that will contain entries
		// need to figure out precise allocation for this
		listOfEntries = malloc(sizeof(entryStruct) * 100);
		/////////////////////////////////////////////
		printf("Signature did NOT match\n");
		vcb_p->blockSize;
		vcb_p->blockCount;
		vcb_p->freeSpaceMap = initFreeSpace(numberOfBlocks, blockSize);
		vcb_p->rootDir = initRootDir();
		vcb_p->signature = MAGICNUMBER;
		LBAwrite(vcb_p, 1, 0);
	}
	else
	{
		printf("Signature Matched\n");
		int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
		printf("fsBlockCount is %d\n", fsBlockCount);
		unsigned char *fsMap = malloc(fsBlockCount * blockSize);
		LBAread(fsMap, fsBlockCount, vcb_p->freeSpaceMap);
	}
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}