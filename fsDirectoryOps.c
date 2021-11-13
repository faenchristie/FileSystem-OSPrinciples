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
    struct dirent *openDir;
    // call b_open
    fd = b_open(name,O_RDWR);
    // set all corresponding values of b_open to fdDir

    if(fd < 0){
        printf("Could not open "%s" directory.\n", name);
        return NULL;
    }

    fdDir dir; 

    for(int i = 0; i < entriesLength; i++) {
        if(strcmp(entries[i].path,name) == 0) {
            printf("Opening "%s" directory... \n", name);
            while( (openDir = fs_readdir(dir) != NULL){
                dir->name = name;
                // // find rec length 
                // // find entry position in directory
                // // find         }

            }
        } else {
            printf("No such "%s" directory found.\n", name);
        }
    
    }    // (length,LBA of directory direntry position, starting LBA of directory, name? )
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
    struct stat * pstat; 
    pstat = (struct stat *) buf;
    // use this to set all values of fs_stat struct
    /** stat() displays file or file system status; returns 0 if successful. */
    /** See: https://pubs.opengroup.org/onlinepubs/009696799/functions/stat.html */
    return (stat(path, pstat));
}
