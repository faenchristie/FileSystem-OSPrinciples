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

#include "fsLow.h"
#include "mfs.h"
#include "fsEntry.h"
#include "fsDirectory.h"
#include "b_io.h"


/******************************************************************************
 *                        -----parses file path-----
 * parsing the file path with the token "/" to denote when the path begins and
 * ends.
 *****************************************************************************/
 char * parsePath(char *path) {
    // nullify path 
    char * filePathArr; 
    char *savePtr;
    char *token = strtok_r(path, '/', &savePtr);
    while(token != NULL){
        strcpy(filePathArr,token);
        token = strtok_r(NULL, "/", &savePtr)
    }
    return filePathArr;
}

/******************************************************************************
 *                   -----opens a directory-----
 * 
 *****************************************************************************/
fdDir *fs_opendir(const char *name)
{
    int fd;
    // call b_open
    fd = b_open(name,O_RDWR);
    // set all corresponding values of b_open to fdDir

    fdDir dir; 

    for(int i = 0; i < entriesLength; i++) {
        if(strcmp(entries[i].path,name)) {
            dir->name = name;
            // find rec length
            // find entry position in directory
            // find         }
    }    // (length,LBA of directory direntry position, starting LBA of directory, name? )



/******************************************************************************
 *                      -----read a directory-----
 * 
 *****************************************************************************/
struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{

    // this function returns the next directory entry of the current directory entry.

    // declare fs_firiteminfo, possibly as global variable?

    // need some sort of way to check which child we are on. possibly the index
    // of the entries? Might need to be global variable.

    // loop through entries, find entry that is the child of this directory.

    // first one found, set the fs_direiteminfo struct properties
    // (length, fileType, name)

    return 0;
}

/******************************************************************************
 *                      -----close a directory-----
 * 
 *****************************************************************************/
int fs_closedir(fdDir *dirp)
{
    // fdDir in mfs.h
    DIR *dir;
    dir = (DIR *) dirp;
    /** Closedir() takes a pointer to a DIR referring to the open directory
     *  stream desired to be closed. A return value of 0 is successful, -1 is
     *  not.
     */
    return (closedir (dir));
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
int fs_isFile(char *path) {
    int entriesLength = sizeof(listOfEntries) / sizeof(entryStruct);
    entryStruct * entry_p ;
    // look for path in entries
    for(int i = 0; i < entriesLength; i++) {
        if(strcmp(entries[i].path,path)){
            entry_p = entries[i];
        }
    }
    
    if(strcmp(entry_p->type, "F"){
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
    int entriesLength = sizeof(listOfEntries) / sizeof(entryStruct);
    entryStruct * entry_p ;
    // look for path in entries
    for(int i = 0; i < entriesLength; i++) {
        if(strcmp(entries[i].path,path)){
            entry_p = entries[i];
        }
    }
    
    if(strcmp(entry_p->type, "D"){
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

    // use this to set all values of fs_stat struct
    return 0;
}
