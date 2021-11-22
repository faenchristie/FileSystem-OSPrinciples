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

#include "fsInit.h"

/**************************************************************************
* --------------------Initializing the file system-------------------------
* First, store a mallaoc of blocksize into the vcb pointer variable,
* then calling an LBAread, with block 0. 
* Next, check if the magic number number in the structure matches with the 
* pointer. If they magic numbers do not match, the initialization will
* occur for the VCB, freespace, and the root directory.
**************************************************************************/
int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize) {

	printf("Initializing File System with %lu blocks with a block size of %lu\n", numberOfBlocks, blockSize);
	printf("About to allocate %ld bytes for VCB\n", sizeof(vcbStruct));

	vcb_p = malloc(5*blockSize);
	
	LBAread(vcb_p, 1, 0);

	if (vcb_p->signature != MAGICNUMBER) {

		printf("Signature did NOT match\n");
		vcb_p->blockSize;
		vcb_p->blockCount;
		vcb_p->freeSpaceMap = initFreeSpace(numberOfBlocks, blockSize);
		vcb_p->rootDir = initRootDir(blockSize);
		vcb_p->rootDirBlocks = sizeof(entryStruct) * DIRENTRIES;
		vcb_p->signature = MAGICNUMBER;
		LBAwrite(vcb_p, 1, 0);
	} else {

		printf("Signature Matched\n");
		int fsBlockCount = (((numberOfBlocks + 7) / 8) + (blockSize - 1)) / blockSize;
		printf("fsBlockCount is %d\n", fsBlockCount);
		freeMap = malloc(fsBlockCount * blockSize);
		LBAread(freeMap, fsBlockCount, vcb_p->freeSpaceMap);
	}
	return 0;
}

void exitFileSystem()
{
	free(vcb_p);
	free(freeMap);
	printf("System exiting\n");
}

