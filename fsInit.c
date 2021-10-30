/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
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

#define MAGICNUMBER = 9823498237

/******************************************************************************
 * specifies information about how many blocks are there, what are the size of
 * each blocks, how many free blocks are there, etc.
 * uint64_t is an unsigned integer of 64 bits
 *****************************************************************************/
typedef struct {
	int blockSize;
	int blockCount;
	int rootDir;
	int freeSpaceMap;
	unsigned char * signature;
} vcbStruct, *vcbStruct_p;

/******************************************************************************
 * GLOBAL VARIABLES HERE
 *****************************************************************************/
int blockLocation;
char name[50];
char type;
int size;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize) {
	/**************************************************************************
	* --------------------Initializing the file system-------------------------
	* First, store a mallaoc of blocksize into the vcb pointer variable,
	* then calling an LBAread, with block 0. 
	* Next, check if the magic number number in the structure matches with the 
	* pointer. If they magic numbers do not match, the initialization will
	* occur for the VCB, freespace, and the root directory.
	**************************************************************************/
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
		/* TODO: Add any code you need to initialize your file system. */

	vcbStruct * vcb_p;
	vcb_p = malloc(blockSize);

	LBAread(vcb_p, 1, 0);

	if (vcb_p->signature != MAGICNUMBER) {
		printf("Signature did NOT match\n");
		vcb_p->blockSize;
		vcb_p->blockCount;
		vcb_p->rootDir = initRoot();
		vcb_p->freeSpaceMap = initFreeSpaceMap(numberOfBlocks, blockSize);
		vcb_p->signature = MAGICNUMBER;
		LBAwrite(vcb_p, 1, 0);
	} else {
		printf("Signature Matched\n");
		int fsBlockCount = (((numberOfBlocks + 7) / 8 ) + (blockSize - 1)) / blockSize;
		printf("fsBlockCount is $d\n", fsBlockCount);
		unsigned char * fsMap = malloc (fsBlockCount * blockSize);
		LBAread(fsMap, fsBlockCount, vcb_p->freeSpaceMap);
	}
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}