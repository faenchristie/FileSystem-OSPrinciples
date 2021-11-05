/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsDirectory.h
*
* Description: holds the directory functions like create 
* directory for now, will use in conjunction with mfs.h later
* on
**************************************************************/

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
int initRootDir();