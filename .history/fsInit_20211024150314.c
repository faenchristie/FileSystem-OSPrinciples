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

/******************************************************************************
 * specifies information about how many blocks are there, what are the size of
 * each blocks, how many free blocks are there, etc.
 * uint64_t is an unsigned integer of 64 bits
 *****************************************************************************/
typedef struct
{
	int magicNum;
	uint64_t volumeSize;
	uint64_t blockSize;
	uint64_t numberOfBlocks;
	uint64_t startingBlock;
	uint64_t numberOfFreeBlocks;
	int freeMapStart;
} vcbStruct, *vcbStruct_p;

/******************************************************************************
 * GLOBAL VARIABLES HERE
 *****************************************************************************/
int initialized = 0; // check if vcb is initialized
uint64_t volumeSize;
uint64_t blockSize;
uint64_t numberOfBlocks;
uint64_t startingBlock;
uint64_t numberOfFreeBlocks;
int freeMapStart;
char *freeSpaceMap;

int blockLocation;
char name[50];
char type;
int size;

vcbStruct *vcb_p;
entryStruct *entry_p;

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
	vcb_p = malloc(blockSize);

	uint64_t read = LBAread(vcb_p, 1, 0);

	if (vcb_p->magicNum == MAGICNUMBER)
	{
		//// vcb already initialized if magic number matches
		printf("VCB initialized...");
	}

	else
	{
		printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
		/* TODO: Add any code you need to initialize your file system. */

		printf("-----Initializing the Volume Control Block-----\n");
		vcb_p->volumeSize = volumeSize;
		vcb_p->blockSize = blockSize;
		vcb_p->numberOfBlocks = numberOfBlocks;
		vcb_p->startingBlock = startingBlock;
		vcb_p->numberOfFreeBlocks = numberOfFreeBlocks;
		vcb_p->magicNum = MAGICNUMBER;

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
		printf("-----Initializing the free space map-----\n");
		freeSpaceMap = malloc(BLOCKSIZE * 5); //free map now has 2560 bytes

		// marked as used
		freeSpaceMap[0] = 1;

		for (int i = 1; i < BLOCKS + 1; i++)
		{
			// mark 1 - BLOCKS as free
			freeSpaceMap[i] = 0;
		}

		LBAwrite(vcb_p, 1, 0);		  // write vcb to block 0
		LBAwrite(freeSpaceMap, 5, 1); // write freeSpaceMap to blocks 1-5
		vcb_p->freeMapStart = 1;

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
		printf("-----Initializing the root directory-----\n");
		entry_p = malloc(DIRENTRIES * 60);

		for (int i = 0; i < DIRENTRIES + 1; i++)
		{
			entry_p[i].blockLocation = 0;
			strcpy(entry_p[i].name, "Entry");
			entry_p[i].type = 'undefined';
			entry_p[i].size = 0;
		}

		strcpy(entry_p[0].name, "Root");
		entry_p[0].blockLocation = BLOCKS + 1;
		entry_p[0].size = 60;
		// entry_p[0].type =

		strcpy(entry_p[1].name, ".");
		entry_p[1].size = entry_p[0].size + 60;
		// entry_p[1].blockLocation =
		// entry_p[1].type =

		strcpy(entry_p[2].name, "..");
		entry_p[2].size = entry_p[1].size;
		// entry_p[2].blockLocation =
		// entry_p[2].type =
	}
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}