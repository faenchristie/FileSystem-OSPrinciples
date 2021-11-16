/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: fsDirectoryOps.c
*
* Description: File contains functions for directory operations 
* including read, close, open, isfile, isDir, stat
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h> // the format of directory entries
#include <sys/stat.h> // Utilizing the stat() function


#include "mfs.h"


/******************************************************************************
 *                        -----parses file path-----
 * parsing the file path with the token "/" to denote when the path begins and
 * ends.
 *****************************************************************************/
 char * parsePath(char *path) {
    // nullify path 
    char * filePathArr;
    printf("parse path error 1");
    char *savePtr;
    printf("parse path error 2");
    char *token = strtok_r(path, '/', &savePtr);
    printf("parse path error 3");
    int i=0;
    while(token != NULL){
        printf("parse path error 4");
        strcpy(filePathArr[i],token);
        printf("parse path error 5"); 
        i++;
        token = strtok_r(NULL, "/", &savePtr);
        printf("parse path error 6");
 
    }
    printf("parse path error 7");
    return filePathArr;
}

/******************************************************************************
 *                        -----returns length of arr-----
 * Takes a char array as input and returns the length of it. This will be
 * used to dissect the parent path. Returns int length.
 *****************************************************************************/

int getArrLength(char *arr)
{
    // tracks length
    int count = 0;
    // will go until null is found
    while (arr[count] != "\0")
    {
        count++;
    }
    return count;
}

/******************************************************************************
 *                        -----gets parent path based on path-----
 * Takes a path parameter of a path. Loops through path by length-1, effectively
 * removing the last part of the path (the child). This parent path can then be
 * used in conjunction with getEntryFromPath to return the entry.
 *****************************************************************************/

 char * getParentPath(char *path){
     char * parentPath;
     // get length of original path
     int pathLength = getArrLength(path);
     // copies in path until the end, removing child
     for(int i=0; i<pathLength-1; i++){
         strcpy(parentPath[i],path[i]);
     }
     return parentPath;
 }

/******************************************************************************
 *               -----Finds entry based on parsed path-----
 * Takes char array with parsed path as parameter and length of array.
 *  Reads through parsed path one name at a time. Starts by opening root directory. 
 * Returns a pointer to an array of entries.
 *****************************************************************************/

entryStruct * getEntryFromPath(char *arr, int arrLength){

    // malloc space needed: size of struct * average entries for directory
    int entrySize = sizeof(entryStruct) * DIRENTRIES;
    entryStruct * entry_p;
    entry_p = malloc(entrySize);

    entryStruct *tempEntry;
    tempEntry = malloc(entrySize);

    // read in root directory into entry_p. 
    LBAread(entry_p,vcb_p->rootDirBlocks,vcb_p->rootDir);

    // if arr length = 0, we are simply returning root.
    if(arrLength==0){
        return entry_p;
    }

    // first for loop loops through each segment of the path
    for(int i=0; i<arrLength; i++){
        // inner for loop loops through each entry of the directory
        for(int j=0; j<DIRENTRIES; j++){

            // ******************** //
            // NOTE: Should I check if value is null to avoid segfault?
            // ******************** //

            // compares the path name to the name of each directory entry
            if(strcmp(entry_p[j].name,arr[i])){
                // read next directory into tempEntry if matches
                LBAread(tempEntry,entry_p[j].blockCount, entry_p[j].blockLocation);
                // move onto next entry
                entry_p = tempEntry;
            }
            else{
                // make entry_p null if not found. This will be used to check
                // if the directory was found. 
                entry_p = NULL;
            }
        }
        // once done looping through, check if next entry was found
        if(entry_p=NULL){
            // no entry of this name was found, hence it is NULL.
            // print error, return.
            printf("DIRECTORY NOT FOUND\n");
            return NULL;
        }
    }

    // free the temp entry
    free(tempEntry);

    return entry_p;
}

/******************************************************************************
 *                   -----opens a directory-----
 * 
 *****************************************************************************/
fdDir *fs_opendir(const char *name)
{
    int fd;
    struct dirent *openDir;
    // call b_open
    fd = b_open(name,O_RDWR);
    // set all corresponding values of b_open to fdDir

    if(fd < 0){
        printf("Could not open %s directory.\n", name);
        return NULL;
    }

    fdDir dir; 
 /*************************************************/
 // UPDATE TO HAVE NEW LOGIC FOR FINDING DIR ENTRY
 /****************************************************/
/*
    for(int i = 0; i < entriesLength; i++) {
        if(strcmp(entries[i].path,name) == 0) {
            printf("Opening %s directory... \n", name);
            while( (openDir = fs_readdir(dir) != NULL){
                dir->name = name;
                // // find rec length 
                // // find entry position in directory
                // // find         }

            }
        } else {
            printf("No such %s directory found.\n", name);
        }
    */
    //}    // (length,LBA of directory direntry position, starting LBA of directory, name? )
    return 0;
}
/******************************************************************************
 *                      -----read a directory-----
 * This function returns the next directory entry of the current directory entry.
 * 
 *****************************************************************************/
int readdirIndex = 0; /** Keeps track of children count */
struct fs_diriteminfo directoryEntry;
struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{
    if(readdirIndex == dirp->childrenAmount){
        readdirIndex = 0;
        printf("Could not read.\n");
        return NULL;
    }

    /** Should fill out diriteminfo */

    /** Increment child index */
    readdirIndex++;

    return &directoryEntry;
}

/******************************************************************************
 *                      -----close a directory-----
 * 
 *****************************************************************************/
int fs_closedir(fdDir *dirp)
{
    printf("Closing directory...\n");
    return 0;
}

       /******************************************************************************
 *  b_close();   -----check if it is a file or not-----
 * First, we created a variable for the length of our array in order to
 * traverse using a for loop. Next, inside the For loop there will need to be
 * a string comparison for the path. Outside the for loop, we have to make sure
 * the types line up from the entries, to do this we compare with a char D
 * which refers to the fact that it is a file. Returning 1 if it is, or 0
 * if it is not a file.
 *****************************************************************************/
        int fs_isFile(char *path)
        {
    
            /*
            int entriesLength = sizeof(listOfEntries) / sizeof(entryStruct);
            entryStruct *entry_p;
            // look for path in entries
            for (int i = 0; i < entriesLength; i++)
            {
                if (strcmp(entries[i].path, path))
                {
                    entry_p = entries[i];
                }
            }*/

            entryStruct *entry_p;

            // parse path
            char *parsedPath = parsePath(path);

            // get length of parsed path (used for next function)
            int pathLength = getArrLength(parsedPath);

            // get entry using functions
            entry_p = getEntryFromPath(parsedPath,pathLength);

    // case insensitive string compare, see if file. return 1 if file
    if(strcasecmp(entry_p->type, "f")){
                return 1;
    }

    else{
                return 0;
    }
        }

   /******************************************************************************
 *                 -----check if it is a directory or not-----                  
 * First, we created a variable for the length of our array in order to
 * traverse using a for loop. Next, inside the For loop there will need to be
 * a string comparison for the path. Outside the for loop, we have to make sure
 * the types line up from the entries, to do this we compare with a char D
 * which refers to the fact that it is a directory. Returning 1 if it is, or 0
 * if it is not a directory.
 *****************************************************************************/
        int fs_isDir(char *path)
        {
            /*
            int entriesLength = sizeof(listOfEntries) / sizeof(entryStruct);
            entryStruct *entry_p;
            // look for path in entries
            for (int i = 0; i < entriesLength; i++)
            {
                if (strcmp(entries[i].path, path))
                {
                    entry_p = entries[i];
                }
            }*/

            entryStruct *entry_p;

            // parse path
            char *parsedPath = parsePath(path);

            // get length of parsed path (used for next function)
            int pathLength = getArrLength(parsedPath);

            // get entry using functions
            entry_p = getEntryFromPath(parsedPath,pathLength);

    if(strcmp(entry_p->type, "D")){
                return 1;
    }

    else{
                return 0;
    }
        }

/******************************************************************************
 * 
 * 
 *****************************************************************************/
int fs_stat(const char *path, struct fs_stat *buf)
{
    // check entries for path , declare that variable
    struct stat * pstat; 
    pstat = (struct stat *) buf;
    // use this to set all values of fs_stat struct
    /** stat() displays file or file system status; returns 0 if successful. */
    /** See: https://pubs.opengroup.org/onlinepubs/009696799/functions/stat.html */
    return (stat(path, pstat));
}


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

    // malloc space needed: size of struct * average entries for directory
    int entrySize = sizeof(entryStruct) * DIRENTRIES;
    entryStruct * entry_p;
    printf("ERROR -2\n");
    entry_p = malloc(entrySize);
    
    printf("ERROR -1\n");

    // get pathname, pathname length
    char *parsedPath = malloc(500);
    printf("ERROR AHHHH\n");
    strcpy(parsedPath,parsePath(pathname));
    printf("ERROR 0\n");
    int pathLength = getArrLength(parsedPath);

    printf("ERROR 1\n");

    int blocksNeeded = ((entrySize + BLOCKSIZE - 1) / BLOCKSIZE);

printf("blocksNeeded %d\n",blocksNeeded);
    int blockNo = 0;

    printf("blockNo %d\n",blockNo);

    // find free block space that will cover the amount needed in bitmap
    blockNo = findFreeBlocks(blocksNeeded);

    printf("ERROR 2\n");

    // error code if not enough memory?

    printf("blockNo %d\n",blockNo);
    
    if (blockNo==0)
    {
        printf("ERROR: NOT ENOUGH MEMORY\n");
        return 0;
    }

    printf("ERROR 3\n");

    // get parent path
    char *parentPath = getParentPath(pathname);
    int parentPathLength = getArrLength(parentPath);

    // get parent entry
    entryStruct * entry_pParent = getEntryFromPath(parentPath,parentPathLength);

    // get parent info, assign values to entry_p 
    entry_p->parent = entry_pParent->blockLocation;
    entry_p->blockLocation = blockNo;
    entry_p->blockCount = blocksNeeded;
    entry_p->type = 'd';
    // the last value of the path is the name of the entry
    strcpy(entry_p->name,parsedPath[pathLength]);

    // write to memory
    LBAwrite(entry_p, blocksNeeded, blockNo);

    printf("ERROR 4\n");

    // update bitmap to signify used space
    for (int i = blockNo; i < blockNo + blocksNeeded; i++)
    {
        printf("blockNo %d , %d\n",blockNo,i);
        freeMap[i] = 1;
    }

    printf("ERROR 5\n");

    // free malloc'd entry now that it is written in memory
    free(entry_p);
    free(entry_pParent);

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
