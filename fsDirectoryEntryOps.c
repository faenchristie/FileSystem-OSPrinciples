/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsDirectoryEntryOps.c
*
* Description: File containing functions for directory entry
* operations including delete, make directory, remove, and cwd
* functions. 
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "mfs.h"
#include "fsInit.c"
#include "fsFree.h"
//#include "fsLow.h"lbawrti
//#include "fsEntry.h"

char currentDirectoryPath[200];

/******************************************************************************
 * -----removes a file-----
 * 
 *****************************************************************************/
int fs_delete(char *filename)
{
    int blocksUsed = 0;
    int blockStart = 0;
    // used to return error if file doesn't exist
    int exists = 0;
    int entryNo = 0;
    // search entry list for file

    //*****************************************************************//
    // CHANGE TO USE NEW DIRECTORY FINDING FUNC
    //******************************************************************//
    /*
    for (int i = 0; i < numberOfEntries; i++)
    {

        // temp solution to avoid checking null value and getting seg fault
        if (listofEntries[i] != '\0')
        {
            // checks if the path matches
        if(strcmp(listOfEntries[i].path,filename)){
                // set exist to true because we found the file
                exists = 1;
                entryNo = i;
                // get info needed to delete entry from bitmap
                blocksUsed = listOfEntries[i].blockCount;
                blocksStart = listOfEntries[i].blockLocation;
        }
        }
    }*/

    // if file not found, print error, exit
    if (!exists)
    {
        printf("ERROR: FILE NOT FOUND\n");
        return 0;
    }

    // if file found, loop through bitmap, marking bits as not used
    if (exists)
    {
        for (int i = blockStart; i < blocksUsed + 1; i++)
        {
            freeMap[i] = 0;
        }
        // nullify entry in list
       // listOfEntries[entryNo] = "\0";

        printf("FILE DELETED\n");
    }

    return 0;
}

/******************************************************************************
 * -----creates a new directory-----
 * 
 *****************************************************************************/
int fs_mkdir(const char *pathname, mode_t mode)
{

    // parse path in some way, find parent path

    // malloc space needed: size of struct * average entries for directory
    int entrySize = sizeof(entryStruct) * DIRENTRIES;
    entryStruct * entry_p;
    entry_p = malloc(entrySize);

    int blocksNeeded = ((entrySize + BLOCKSIZE - 1) / BLOCKSIZE);

    // find free block space that will cover the amount needed in bitmap
    int blockNo = findFreeBlocks(blocksNeeded);

    // error code if not enough memory?
    if (!blockNo)
    {
        printf('ERROR: NOT ENOUGH MEMORY\n');
        return 0;
    }

    // write to memory
    LBAwrite(entry_p, blocksNeeded, blockNo);

    // update bitmap to signify used space
    for (int i = blockNo; i < blocksNeeded + 1; i++)
    {
        freeMap[i] = 1;
    }

    // assign all necessary values to the entry

    // FIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // add it to list
    //listOfEntries[numberOfEntries] = entry_p;

    // increment entry count for list
    //numberOfEntries++;

    return 0;
}

/******************************************************************************
 * -----remove a directory-----
 * 
 *****************************************************************************/
int fs_rmdir(const char *pathname)
{
    // block that entry starts at
    int blockNo;
    // total blocks it uses
    int totalBlocks;
    
    //int entryNo;
    // bool to check if path exists
    int found = 0;

    // parse file path
    char *parsedPath = parsePath(pathname);

    // get length of array (used for getentry function)
    int pathLength = getArrLength(parsedPath);

    entryStruct *entry_p = getEntryFromPath(parsedPath, pathLength);

    if(entry_p!=NULL){
        found = 1;
    }



    /************************************************************************/
    // CHANGE TO USE NEW FIND DIR FUNC
    /*************************************************************************/
/*
    // find directory of the pathname in our list of entries
    for (int i = 0; i < numberOfEntries; i++)
    {
        // temp solution to avoid checking null value and getting seg fault
        if (listofEntries[i] != '\0')
        {
            if (strcmp(pathname, listOfEntries[i].path))
            {
                blockNo = listOfEntries[i].blockLocation;
                totalBlocks = listOfEntries[i].blockCount;
                entryNo = i;
                // set found to true
                found = 1;
            }
        }
    }*/

    // if directory is not found, print error, return
    if (!found)
    {
        printf("DIRECTORY DOES NOT EXIST\n");
        return 0;
    }

    // print if deleted
    printf("DIRECTORY DELETED\n");

    // mark bitmap space to 0 to signify space is free
    for (int i = blockNo; i < totalBlocks + 1; i++)
    {
        freeMap[i] = 0;
    }

    return 0;
}

/******************************************************************************
 * 
 * 
 *****************************************************************************/
char *fs_getcwd(char *buf, size_t size)
{
    if (strlen(currentDirectoryPath > size))
    {
        // sizes too long, print error
        printf("ERROR PATH TOO LONG\n");
        return 0;
    }

    // copy path of cwd to buffer to be returned
    strcpy(buf, currentDirectoryPath);

    return buf;
}

/******************************************************************************
 * 
 * 
 *****************************************************************************/
int fs_setcwd(char *buf)
{
    /// ***************** WORK ON THIS ******************///
    // parse file path

    // search for directory based on path
    // boolean
    int found = 0;

    //*****************************************************************//
    // CHANGE TO USE NEW FIND DIR FUNC
    //******************************************************************//

    /*
    for (int i = 0; i < numberOfEntries; i++)
    {
        // temp solution to avoid checking null value and getting seg fault
        if (listofEntries != '\0')
        {
            if (strcmp(listOfEntries[i].path, buf))
            {
                found = 1;
            }
        }
    }*/

    // DIRECTORY NOT FOUND
    if (!found)
    {
        printf("DIRECTORY NOT FOUND\n");
        return 0;
    }

    // clear old path
    /**********************************************/
    // FIND PROPER WAY TO CLEAR
    /*********************************************/
    // currentDirectoryPath = NULL;

    // set new CWD
    strcpy(currentDirectoryPath, buf);

    return 1;
}
