/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsFree.h
*
* Description: function definitions for initializing the free
* space for the file system.
**************************************************************/

int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);

int findFreeBlocks(int blocks);