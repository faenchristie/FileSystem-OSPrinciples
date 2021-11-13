/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsDirectory.c
*
* Description: logic for the initRoot() function and also
* creation of the listOfEntries array
**************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
//#include "fsEntry.h"
#include "fsDirectory.h"
#include "fsFree.h"
#include "fsInit.c"

entryStruct *entry_p;

// some sort of way to keep track of our entries?
//array of directory entries witht he size 5 * 512 which is 2,560 bytes

/**********************************************************************
* -----------------Initializing the root directory---------------------
* First, there will need to be a malloc for the root directory which is
* 3000 bytes. Next, Creating a pointer to an array of directory entries
* and initializing them to be in a free state.
* Next, the first directory entry will be "." which will be of size
* 3060 bytes. It will begin at block number ...
* Next, the second directory entry will be ".." which will be the same 
* as the first entry "." except that the name will be ".."
* Finally, return the starting block number of the root directory
**********************************************************************/
int initRootDir()
{
	printf("-----Initializing the root directory-----\n");

	// size of entry multiplied by defined average entries
	int entrySize = sizeof(entryStruct) * DIRENTRIES;

	entry_p = malloc(entrySize);

	// number of blocks needed for root dir based on malloc'd space.
	// ceiling division
	int blocksNeeded = ((entrySize + BLOCKSIZE - 1) / BLOCKSIZE);

	// find free space
	int free = findFreeBlocks(blocksNeeded); // parameter is amount of blocks needed for root

    // "." directory, or first entry, is just a pointer to the current directory.
	strcpy(entry_p[0].name,".");
	// block location of root is the free space we found.
	entry_p[0].blockLocation = free;
	// amount of blocks root takes up 
	entry_p[0].blockCount = blocksNeeded; 
	entry_p[0].size = entrySize;
	entry_p[0].type = 'd'; // directory
	// path of root directory
	entry_p[0].path="/";

	// second entry is the same as first because root does not have a parent 
	// except the name is ".."
	strcpy(entry_p[1].name,"..");
    entry_p[1].blockLocation = free;
	entry_p[1].blockCount = blocksNeeded;
	entry_p[1].size = entrySize;
	entry_p[1].type = 'd';
	entry_p[1].path="/";

	// other directories will be initialized when made.
	// when you make a directory, you will update block location and other
	// necessary information, and marked them as used.
	for(int i=2; i< DIRENTRIES-2; i++){
		// marked as undefined. We can use this to see what entries
		// are being used inside our directory.
		entry_p[i].type = 'u';
	}
	
     /*
	for (int i = 0; i < DIRENTRIES + 1; i++)
	{
		entry_p[i].blockLocation = free; // location written into memory
		strcpy(entry_p[i].name, "Entry");
		entry_p[i].type = 'u'; // U = undefined
		entry_p[i].size = 0;
	}

	printf("After init entries");

	strcpy(entry_p[0].name, "Root");
	entry_p[0].blockLocation = BLOCKS + 1;
	// change later
	entry_p[0].size = 60;
	entry_p[0].blockCount = blocksNeeded;
	entry_p[0].type = 'D';

	strcpy(entry_p[1].name, ".");
	// change later
	entry_p[1].size = entry_p[0].size + 60;
	// entry_p[1].blockLocation =
	entry_p[1].type = 'D';

	strcpy(entry_p[2].name, "..");
	entry_p[2].size = entry_p[1].size;
	// entry_p[2].blockLocation =
	entry_p[2].type = 'D';*/

	//writing root directory, "blocksNeeded" blocks starting at the free space returned to us by map
	LBAwrite(entry_p, blocksNeeded, free);

	/////////////////////////////////////
	// add to entry list
	//listOfEntries[0] = entry_p;
	// increment amount of entries
	//numberOfEntries++;
	/////////////////////////////////////

	// return location of root for VCB to store
	return free;
}
