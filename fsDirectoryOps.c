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
//#include "fsEntry.h"
#include "fsInit.c"
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
        token = strtok_r(NULL, "/", &savePtr);
    }
    return filePathArr;
}

/******************************************************************************
 *                        -----get arr length-----
 * This will be used to dissect the parent path.
 * Takes a char array as input and returns the length of it. 
 * set the count to 0 to track the length
 * a while loop will trigger until null is found
 * finally it will return the count, int length
 *****************************************************************************/

int getArrLength(char *arr) {

    int count = 0;

    while (arr[count] != "\0") {
        count++;
    }
    return count;
}

/******************************************************************************
 *               -----Finds entry based on parsed path-----
 * Takes char array with parsed path as parameter and length of array.
 * Reads through parsed path one name at a time. Starts by opening root directory. 
 * Returns a pointer to an array of entries.
 *****************************************************************************/

entryStruct * getEntryFromPath(char *arr, int arrLength){

    entryStruct *entry_p;
    entryStruct *tempEntry;

    // read in root directory into entry_p. 
    LBAread(entry_p,vcb_p->rootDirBlocks,vcb_p->rootDir);

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

    return entry_p;
}

/******************************************************************************
 *                      -----open directory-----
 * This function will open the designated directory from the parameter
 * 
 * create a file descriptor variable of type int
 * create pointer to openDir
 * call b_open function
 * set all corresponding values of b_open to fdDir
 * create error condition
 * return 0
 *****************************************************************************/
fdDir *fs_opendir(const char *name) {
    int fd;
    struct dirent *openDir;

    fd = b_open(name,O_RDWR);
    // set all corresponding values of b_open to fdDir

    if(fd < 0){
        printf("Could not open %s directory.\n", name);
        return NULL;
    }

    fdDir dir; 
    return 0;
}
/*************************************************
* UPDATE TO HAVE NEW LOGIC FOR FINDING DIR ENTRY
****************************************************/
/******************************************************************************
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
    }    (length,LBA of directory direntry position, starting LBA of directory, name? )
******************************************************************************/

/******************************************************************************
 *                      -----read a directory-----
 * This function returns the next directory entry of the current directory entry.
 * 
 *****************************************************************************/
int childReadIndex = 0; /** Keeps track of children count */
struct fs_diriteminfo directoryEntry;

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    if(childReadIndex == dirp->childrenAmount){
        childReadIndex = 0;
        printf("Could not read.\n");
        return NULL;
    }

    /** Should fill out diriteminfo */

    /** Increment child index */
    childReadIndex++;

    return &directoryEntry;
}

/******************************************************************************
 *                      -----close a directory-----
 * This function closes a directory, it simply prints out a prompt so the 
 * user knows the directory is being closed, then returns 0
 *****************************************************************************/
int fs_closedir(fdDir *dirp) {
    printf("Closing directory...\n");
    return 0;
}

/******************************************************************************
 *                -----check if it is a file or not-----
 * create a pointer to the entry
 * create a variable parsedpath which parses the path
 * create a variable path length which stores the length of the path
 * call getEntryFromPath and store in entry_p
 * string comparison of the type in struct and "F"
 * return 1 if it is true, 0 if not
 * 
 *                          ----OLD----
 * int entriesLength = sizeof(listOfEntries) / sizeof(entryStruct);
 * entryStruct *entry_p;
 * // look for path in entries
 * for (int i = 0; i < entriesLength; i++) {
 *  if (strcmp(entries[i].path, path)) {
 *      entry_p = entries[i];
 *      }
 *  }
 *****************************************************************************/
int fs_isFile(char *path) {
    
    entryStruct *entry_p;
    char *parsedPath = parsePath(path);
    int pathLength = getArrLength(parsedPath);
    entry_p = getEntryFromPath(parsedPath, pathLength);

    if(strcasecmp(entry_p->type, "F")){
        return 1;
    } else{
        return 0;
    }
}

/******************************************************************************
 *                 -----check if it is a directory or not-----                  
 * create a pointer to the entry
 * create a variable parsedpath which parses the path
 * create a variable path length which stores the length of the path
 * call getEntryFromPath and store in entry_p
 * string comparison of the type in struct and "D"
 * return 1 if it is true, 0 if not
 * 
 *                              ----OLD----
 * int entriesLength = sizeof(listOfEntries) / sizeof(entryStruct);
 * entryStruct * entry_p;
 * 
 * for (int i = 0; i < entriesLength; i++) {
 *      if (strcmp(entries[i].path, path)) {
 *          entry_p = entries[i];
 *      }
 *  }
 *****************************************************************************/
int fs_isDir(char *path) {

    entryStruct *entry_p;
    char *parsedPath = parsePath(path);
    int pathLength = getArrLength(parsedPath);
    entry_p = getEntryFromPath(parsedPath, pathLength);

    if(strcmp(entry_p->type, "D")){
        return 1;
    } else{
        return 0;
    }
}

/******************************************************************************
 *                      -----file system stat-----
 * sets all the values for the fs_stat struct
 * check the entries for the path declaring the variable
 * call stat() to display the file or file system status and set all values
 * returns 0 if the function call was sucessful.
 * 
 * source: https://pubs.opengroup.org/onlinepubs/009696799/functions/stat.html
 *****************************************************************************/
int fs_stat(const char *path, struct fs_stat *buf) {
    // check entries for path , declare that variable
    struct stat * pstat; 
    pstat = (struct stat *) buf;

    return (stat(path, pstat));
}
