/**************************************************************
* Class: CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsFree.c
*
* Description: Initialization of the free space map
**************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fsFree.h"

vcbStruct * vcb_p;
/**********************************************************************
* -----------------initializing the free space map---------------------
* will first malloc the freespace map to be blocksize * 5 to allocate a
* total of 2560 bytes. Next will mark which blocks are used and not 
* used, block 0 is reserved for the VCB while block 1-5 inclusive will
* be utilized for the free space map that was created. Next, will call 
* an LBAwrite to write the data to disk, and then finally return the 
* starting block number of the free space to the VCB initialization.
* to indicate where the free space starts.
**********************************************************************/
int initFreeSpace(numberOfBlocks, blockSize) {
    printf("-----Initializing the free space map-----\n");
	vcb_p->freeSpaceMap = malloc(blockSize * 5); //free map now has 2560 bytes

	// marked as used
	vcb_p->freeSpaceMap[0] = 1;

	for (int i = 1; i < numberOfBlocks + 1; i++)
	{
		// mark 1 - BLOCKS as free
		vcb_p->freeSpaceMap[i] = 0;
	}

	LBAwrite(vcb_p, 1, 0);		  // write vcb to block 0
	LBAwrite(vcb_p->freeSpaceMap, 5, 1); // write freeSpaceMap to blocks 1-5
	vcb_p->freeMapStart = 1;
}