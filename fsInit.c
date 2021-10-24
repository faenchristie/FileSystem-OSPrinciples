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
#include "fsVCB.h"
#include "fsEntry.h"

/******************************************************************************
 * VARIABLES HERE
 *****************************************************************************/
int initialized = 0; // check if vcb is initialized
uint64_t volumeSize;
uint64_t blockSize;
uint64_t numberOfBlocks;
uint64_t startingBlock;
uint64_t numberOfFreeBlocks;
uint64_t freeListSize;
vcbStruct *vcb_p;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	printf("-----Initializing the Volume Control Block-----\n");
	vcb_p->volumeSize = volumeSize;
	vcb_p->blockSize = blockSize;
	vcb_p->numberOfBlocks = numberOfBlocks;
	vcb_p->startingBlock = startingBlock;
	vcb_p->numberOfFreeBlocks = numberOfFreeBlocks;
	vcb_p->freeListSize = freeListSize;
	// "Malloc a Block of memory as your VCB pointer"
	vcb_p = malloc(blockSize);

	// "LBAread block 0"
	uint64_t read = LBAread(vcb_p, 1, 0);

	//// INITIALIZE FREE SPACE ///

	//////////////////////////////

	/////// INITIALIZE ROOT //////

	//////////////////////////////

	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}