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
* Description: holds the directory functions like create 
* directory for now, will use in conjunction with mfs.h later
* on
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
#include "fsFree.c"

entryStruct * entry_p;

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
int initRootDir() {
    printf("-----Initializing the root directory-----\n");
	entry_p = malloc(3000);

	for (int i = 0; i < DIRENTRIES + 1; i++)
	{
		entry_p[i].blockLocation = 0;
		strcpy(entry_p[i].name, "Entry");
		entry_p[i].type = 'undefined';
		entry_p[i].size = 0;
	}

	printf("After init entries");

	strcpy(entry_p[0].name, "Root");
	entry_p[0].blockLocation = BLOCKS + 1;
	entry_p[0].size = 60;
	entry_p[0].type = 'D';

	printf("After root");

	strcpy(entry_p[1].name, ".");
	entry_p[1].size = entry_p[0].size + 60;
	// entry_p[1].blockLocation = 
	entry_p[1].type = 'D';

	printf("After entry 1");

	strcpy(entry_p[2].name, "..");
	entry_p[2].size = entry_p[1].size;
	// entry_p[2].blockLocation =
	entry_p[2].type = 'D';

    printf("After entry 2");
}
