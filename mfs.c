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
 void parsePath(char *path) {
     // get length of parsedPath, traverse through, nullify
    int arrLength = getArrLength(parsedPath);
    printf("arrlength, %i\n",arrLength);
    for(int i=0; i<arrLength; i++){
        parsedPath[i] = "\0";
    }
    printf("parse path error 1\n");
    char *savePtr;
    printf("parse path error 2\n");
    char *delim = "/";
    char *token = strtok_r(path, delim, &savePtr);
    printf("parse path error 3\n");
    int i=0;
    while(token != NULL){
        printf("parse path error 4\n");
        parsedPath[i] = token;
        printf("parse path error 5\n"); 
        i++;
        token = strtok_r(NULL, delim, &savePtr);
        printf("parse path error 6\n");
     
    }
    printf("parse path error 7\n");
}

/******************************************************************************
 *                        -----returns length of arr-----
 * Takes a char array as input and returns the length of it. This will be
 * used to dissect the parent path. Returns int length.
 *****************************************************************************/

int getArrLength(char *arr[])
{
    printf("GET ARR LENGTH FUNCTION\n");
    // tracks length
    int count = 0;
    // will go until null is found
    while (arr[count]!=('\0'||NULL))
    {
        printf("GET ARR LENGTH FUNCTION 2\n");
        count++;
    }
    printf("END ARR LENGTH FUNCTION\n");
    return count;
}

/******************************************************************************
 *                        -----gets parent path based on path-----
 * Takes a path parameter of a path. Loops through path by length-1, effectively
 * removing the last part of the path (the child). This parent path can then be
 * used in conjunction with getEntryFromPath to return the entry.
 *****************************************************************************/

void getParentPath(){
    printf("get Parent Path Function\n");
    // nullify past parent path
    int arrLength = getArrLength(parentPath);
    printf("error 1\n");
    for(int i=0; i<arrLength; i++){
        parentPath[i] = "\0";
    }
     // get length of original path
     int pathLength = getArrLength(parsedPath);
     printf("path length: %i\n",pathLength);
     // copies in path until the end, removing child
     for(int i=0; i<pathLength-1; i++){
        printf("Path[i], %s\n",parsedPath[i]);
        parentPath[i] = parsedPath[i];
     }
     int parentPathLength = getArrLength(parentPath);
     printf("Parent Path length: %i\n",parentPathLength);
 }

/******************************************************************************
 *               -----Finds entry based on parsed path-----
 * Takes char array with parsed path as parameter and length of array.
 *  Reads through parsed path one name at a time. Starts by opening root directory. 
 * Returns a pointer to an array of entries.
 *****************************************************************************/

void getEntryFromPath(char *arr[], int arrLength){
    printf("arrLength is %i :\n",arrLength);
    printf("GETENTRYFROMPATH REACHED\n");
    // malloc space needed: size of struct * average entries for directory
    int entrySize = sizeof(entryStruct) * DIRENTRIES;
    currentEntry = malloc(entrySize);
    printf("GETENTRY ERROR 1\n");
    entryStruct *tempEntry;
    tempEntry = malloc(entrySize);
    printf("GETENTRY ERROR 2\n");
    // read in root directory into entry_p. 
    LBAread(tempEntry,vcb_p->rootDirBlocks,vcb_p->rootDir);
    printf("GETENTRY ERROR 3\n");
    // if arr length = 0, we are simply returning root.
    if(arrLength==0){
        currentEntry = tempEntry;
        //free(tempEntry);
        return;
    }

    printf("GETENTRY ERROR 4\n");
    // first for loop loops through each segment of the path
    for(int i=0; i<=arrLength; i++){
        printf("GETENTRY ERROR 5\n");
        // inner for loop loops through each entry of the directory
        for(int j=0; j<DIRENTRIES; j++){
            printf("GETENTRY ERROR 6\n");
            // ******************** //
            // NOTE: Should I check if value is null to avoid segfault?
            // ******************** //
             printf("tempEntry name: %s\n",tempEntry[j].name);
            printf("arr[0]: %s\n",arr[0]);
            // compares the path name to the name of each directory entry
            if(tempEntry[j].name==arr[i]){
               
                printf("GETENTRY ERROR 7\n");
                // read next directory into tempEntry if matches
                LBAread(tempEntry,currentEntry[j].blockCount, currentEntry[j].blockLocation);
                printf("GETENTRY ERROR 8\n");
                // move onto next entry
                currentEntry = tempEntry;
                printf("GETENTRY ERROR 9\n");
            }
            else{
                // make entry_p null if not found. This will be used to check
                // if the directory was found. 
                printf("GETENTRY ERROR 10\n");
                currentEntry = NULL;
                printf("GETENTRY ERROR 11\n");
            }
        }
        // once done looping through, check if next entry was found
        if(currentEntry==NULL){
            // no entry of this name was found, hence it is NULL.
            // print error, return.
            printf("DIRECTORY NOT FOUND\n");
            return;
        }
    }
    printf("Current entry block location: %i\n",currentEntry[0].blockLocation);
    printf("GETENTRY ERROR 11\n");
    // free the temp entry
    free(tempEntry);
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
            //char *parsedPath = parsePath(path);

            // get length of parsed path (used for next function)
            //int pathLength = getArrLength(parsedPath);

            // get entry using functions
            //entry_p = getEntryFromPath(parsedPath,pathLength);

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
            //char *parsedPath = parsePath(path);

            // get length of parsed path (used for next function)
            int pathLength; //= getArrLength(parsedPath);

            // get entry using functions
            //entry_p = getEntryFromPath(parsedPath,pathLength);

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
    printf("ERROR A\n");
    parsePath(pathname);
    printf("ERROR 0\n");
    int pathLength;
    pathLength = getArrLength(parsedPath);

    printf("ERROR 1\n");

    printf("Path 0 = %s\n",parsedPath[0]);

    printf("PathLength %i",getArrLength(parsedPath));

    int blocksNeeded = ((entrySize + BLOCKSIZE - 1) / BLOCKSIZE);
   
    int blockNo = 0;

    printf("blocksNeeded %d\n",blocksNeeded);

    printf("blockNo %d\n",blockNo);

    // find free block space that will cover the amount needed in bitmap

    blockNo = findFreeBlocks(blocksNeeded);

    printf("blockNo, %i\n",blockNo);

    printf("ERROR 2\n");

    printf("blockNo %d\n",blockNo);
    
    if (blockNo==0)
    {
        printf("ERROR: NOT ENOUGH MEMORY\n");
        return 0;
    }

    printf("ERROR 3\n");

    // get parent path
    getParentPath(pathname);
    printf("ERROR AFTER GETPARENTPATH\n");
    int parentPathLength = getArrLength(parentPath);
    printf("ERROR AFTER PARENTPATHLENGTH\n");
    // get parent entry
    getEntryFromPath(parentPath,parentPathLength);
    if(currentEntry==NULL){
        printf("DIRECTORY NOT FOUND\n");
        return -1;
    }
    // get parent info, assign values to entry_p 
    printf("parent location: %i\n",currentEntry[0].blockLocation);
    printf("parent count: %i\n",currentEntry[0].blockCount);
    printf("parent name: %s\n",currentEntry[0].name);
    // first entry is just a pointer to itself
    entry_p[0].parent = currentEntry[0].blockLocation;
    entry_p[0].blockLocation = blockNo;
    entry_p[0].blockCount = blocksNeeded;
    strcpy(entry_p[0].name, ".");
    strcpy(entry_p[0].type,"d");

    // next entry is pointer to parent
    entry_p[1].parent = currentEntry[0].parent;
    entry_p[1].blockLocation = currentEntry[0].blockLocation;
    entry_p[1].blockCount = currentEntry[0].blockCount;
    strcpy(entry_p[1].name,"..");
    strcpy(entry_p[1].type,"d");

    // initialize other values as undefined entry
    for(int i=2; i<DIRENTRIES-2; i++){
        strcpy(entry_p[i].type,"u");
    }
    printf("PARSED PATH: %s\n",parsedPath[pathLength-1]);

        printf("currentEntry[0].type:%s\n",currentEntry[0].type[0]);
    for(int i=0; i<DIRENTRIES-1; i++){
        printf("currentEntry[%i].type:%s\n",i,currentEntry[i].type[0]);
        if(currentEntry[i].type=="u"){
            // set current Entry [i] as our new entry
            currentEntry[i].blockLocation = blockNo;
            currentEntry[i].blockCount = blocksNeeded;
            strcpy(currentEntry[i].type,"d");
            // set name to the last item in the parsed pathh arr
            strcpy(currentEntry[i].name,parsedPath[pathLength]);
            // write current entry to memory with new values
            LBAwrite(currentEntry,currentEntry[0].blockCount,currentEntry[0].blockLocation);
            // break from loop
            break;
        }
    }

    // the last value of the path is the name of the entry
    //strcpy(entry_p->name,parsedPath[pathLength]);

    // write new entry to memory
    LBAwrite(entry_p, blocksNeeded, blockNo);

    printf("ERROR 4\n");

    // update bitmap to signify used space
    for (int i = blockNo; i < blockNo + blocksNeeded; i++)
    {
        freeMap[i] = 1;
    }

    printf("ERROR 5\n");

    // free malloc'd entry now that it is written in memory
    free(currentEntry);
    free(entry_p);

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
    //char *parsedPath = parsePath(pathname);

    // get length of array (used for getentry function)
    //int pathLength = getArrLength(parsedPath);

    entryStruct *entry_p; //= getEntryFromPath(parsedPath, pathLength);

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
