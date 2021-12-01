/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsRoot.c
*
* Description: logic for the initRoot() function
**************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "fsRoot.h"
#include "fsInit.h"

entryStruct *entry_p;

/**********************************************************************
 *			   -----Initializing the root directory-----
 *
 * First, there will need to be a malloc for the root directory which is
 * 3000 bytes. Next, Creating a pointer to an array of directory entries
 * and initializing them to be in a free state.
 * Next, the first directory entry will be "." which will be of size
 * 3060 bytes. It will begin at block number ...
 * Next, the second directory entry will be ".." which will be the same 
 * as the first entry "." except that the name will be ".."
 * Finally, return the starting block number of the root directory
 *
 * @return freeBlock
 **********************************************************************/
int initRootDir(uint64_t blockSize) {
	printf("-----Initializing the root directory-----\n");
		
	// size of entry multiplied by defined average entries
	int entrySize = (sizeof(entryStruct) * DIRENTRIES);

	entry_p = malloc(entrySize);

	// number of blocks needed for root dir based on malloc'd space.
	// ceiling division
	int blocksNeeded =  ((entrySize + blockSize - 1) / blockSize);

	// find free space
	int freeBlock = findFreeBlocks(blocksNeeded); // parameter is amount of blocks needed for root

    for (int i = freeBlock; i < freeBlock + blocksNeeded; i++) {
		// mark free map used 
        freeMap[i] = 1;
    }

    // "." directory, or first entry, is just a pointer to the current directory.
	strcpy(entry_p[0].name,".");
	// block location of root is the free space we found.
	entry_p[0].blockLocation = freeBlock;
	// amount of blocks root takes up 
	entry_p[0].blockCount = blocksNeeded; 
	entry_p[0].size = entrySize;
	entry_p[0].type = 1; // directory
	entry_p[0].childrenAmount = 0;

	// second entry is the same as first because root does not have a parent 
	// except the name is ".."
	strcpy(entry_p[1].name,"..");
    entry_p[1].blockLocation = freeBlock;
	entry_p[1].blockCount = blocksNeeded;
	entry_p[1].size = entrySize;
	entry_p[1].type = 1;

	// other directories will be initialized when made.
	// when you make a directory, you will update block location and other
	// necessary information, and marked them as used.
	
	for(int i = 2; i < DIRENTRIES; i++) {
		// marked as undefined. We can use this to see what entries
		// are being used inside our directory.
		entry_p[i].type = 0;
		strcpy(entry_p[i].name,"entry");
	}

	//writing root directory, "blocksNeeded" blocks starting at the free space returned to us by map
	LBAwrite(entry_p, blocksNeeded, freeBlock);

	// free malloc'd space
	free(entry_p);

	// set cwd to root
	//fs_setcwd("/");

	// return location of root for VCB to store
	return freeBlock;
}