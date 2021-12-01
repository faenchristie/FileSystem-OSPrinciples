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
#include "fsFree.h"
#include "fsInit.h"

int freeMapSize = 2560;

/******************************************************************************
 * 						   -----find free blocks-----
 * 
 * created two variables to hold our freespacce location and to keep track of
 * the free blocks
 * Then loop through the freemap and if its found, set start to the index
 * next inner loop will begin to keep track of how many free spaces we have in
 * a row.
 * if enough freeblocks are found at this location break the loop
 * reinitialize freeblocks for next inner loop
 * 
 * @return freeStart
 *****************************************************************************/
int findFreeBlocks(int blocksNeeded) {
	int freeStart = 0; //free space location
	int freeBlocks = 0;

	for(int i=0; i<freeMapSize; i++) {
		// if found
		if(freeMap[i] == 0) {
			freeStart = i;
			// inner loop will begin. This will keep track of how many free spaces 
			// we have in a row
			for(int j =freeStart + 1; j < freeMapSize; j++) {
				// break inner for loop either when we found a used space
				// or we found enough freeBlocks.
				if(freeMap[j] == 1 || blocksNeeded == freeBlocks) {
					break;
				}
				freeBlocks++;
			}
		}
		// if enough freeBlocks are found at this freeStart location, break 
		if(freeBlocks == blocksNeeded) {
			break;
		}

		freeBlocks = 0;
		freeStart++;
	}
	return freeStart;
}

/**********************************************************************
* 			  -----initializing the free space map-----
*
* will first malloc the freespace map to be blocksize * 5 to allocate a
* total of 2560 bytes. Next will mark which blocks are used and not 
* used, block 0 is reserved for the VCB while block 1-5 inclusive will
* be utilized for the free space map that was created. Next, will call 
* an LBAwrite to write the data to disk, and then finally return the 
* starting block number of the free space to the VCB initialization.
* to indicate where the free space starts.
*
* @return 1
**********************************************************************/
int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize) {
	printf("-----Initializing the free space map-----\n");

	freeMap = malloc(blockSize * 5); // free map now has 2560 bytes

	// marked as used
	freeMap[0] = 1;

	for (int i = 0; i < 6; i++) {
		freeMap[i] = 1; // mark 1-5 Blocks used, for VCB and freemap
	}

	for (int i = 6; i < blockSize*5; i++) {
		// mark 1 - BLOCKS as free
		freeMap[i] = 0;
	}

	LBAwrite(vcb_p, 1, 0);	 // write vcb to block 0
	LBAwrite(freeMap, 5, 1); // write freeSpaceMap to blocks 1-5

	return 1; // return where free space map starts
}