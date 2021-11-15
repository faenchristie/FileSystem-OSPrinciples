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
int freeMapSize = 2560;

// fix this
// we need to find how large this must be
// potentially malloc/realloc?
//entryStruct *listOfEntries;
//int numberOfEntries = 0;


// finds next free block in freespace map, returns it
// parameter is how much blocks are required (the size of the file)
int findFreeBlocks(int blocksNeeded) {
	// this is the variable we will hold our free space location at 
	int freeStart = 0;
	// this will keep track of if there are enough free blocks at that location
	int freeBlocks = 0;

	// loop through free map
	for(int i=0; i<freeMapSize; i++){
		// if we found a free space, start inner loop
		if(freeMap[i]=0){
			// inner loop will begin. This will keep track of how many free spaces we have
			// IN A ROW.
			for(int j=freeStart+1; j<freeMapSize; j++){
				// break inner for loop either when we found a used space
				// or we found enough freeBlocks. 
				if(freeMap[j]==1||blocksNeeded==freeBlocks){
					break;
				}
				freeBlocks++;
			}
		}
		// if enough freeBlocks are found at this freeStart location, break loop
		if(freeBlocks=blocksNeeded){
			break;
		}
		// reinitialize freeBlocks for next inner loop.
		freeBlocks = 0;
		freeStart++;
	}
	// return the start of our free spot.
	return freeStart;
}

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
int initRootDir(uint64_t blockSize)
{
	printf("-----Initializing the root directory-----\n");
		
	// size of entry multiplied by defined average entries
	int entrySize = (sizeof(entryStruct) * DIRENTRIES);

	entry_p = malloc(entrySize);

	// number of blocks needed for root dir based on malloc'd space.
	// ceiling division
	int blocksNeeded =  ((entrySize + blockSize - 1) / blockSize);

	// find free space
	int freeBlock = findFreeBlocks(blocksNeeded); // parameter is amount of blocks needed for root

	

    // "." directory, or first entry, is just a pointer to the current directory.
	strcpy(entry_p[0].name,".");
	// block location of root is the free space we found.
	entry_p[0].blockLocation = freeBlock;
	// amount of blocks root takes up 
	entry_p[0].blockCount = blocksNeeded; 
	entry_p[0].size = entrySize;
	entry_p[0].type = 'd'; // directory
	// path of root directory
	entry_p[0].path="/";

	// second entry is the same as first because root does not have a parent 
	// except the name is ".."
	strcpy(entry_p[1].name,"..");
    entry_p[1].blockLocation = freeBlock;
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
	LBAwrite(entry_p, blocksNeeded, freeBlock);

	/////////////////////////////////////
	// add to entry list
	//listOfEntries[0] = entry_p;
	// increment amount of entries
	//numberOfEntries++;
	/////////////////////////////////////

	// free entry_p
	//free(entry_p);

	// return location of root for VCB to store

	free(entry_p);

	return freeBlock;
}


int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("-----Initializing the free space map-----\n");

	freeMap = malloc(blockSize * 5); // free map now has 2560 bytes

	// marked as used
	freeMap[0] = 1;

	for (int i = 0; i < 6; i++)
	{
		freeMap[i] = 1; // mark 1-5 Blocks used, for VCB and freemap
	}

	for (int i = 6; i < blockSize*5; i++)
	{
		// mark 1 - BLOCKS as free
		freeMap[i] = 0;
	}

	LBAwrite(vcb_p, 1, 0);	 // write vcb to block 0
	LBAwrite(freeMap, 5, 1); // write freeSpaceMap to blocks 1-5

	return 1; // return where free space map starts
}

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

	if (vcb_p->signature != MAGICNUMBER)
	{
		////////////// CHANGE LATER /////////////////
		// list that will contain entries
		// need to figure out precise allocation for this
		//listOfEntries = malloc(sizeof(entryStruct) * 100);
		/////////////////////////////////////////////
		printf("Signature did NOT match\n");
		vcb_p->blockSize;
		vcb_p->blockCount;
		vcb_p->freeSpaceMap = initFreeSpace(numberOfBlocks, blockSize);
		vcb_p->rootDir = initRootDir(blockSize);
		vcb_p->rootDirBlocks = sizeof(entryStruct) * DIRENTRIES;
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
	free(vcb_p);
	free(freeMap);
	printf("System exiting\n");
}
