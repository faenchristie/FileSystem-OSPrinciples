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
#include <dirent.h>   // the format of directory entries
#include <sys/stat.h> // Utilizing the stat() function

#include "fsLow.h"
#include "mfs.h"
#include "fsEntry.h"
#include "fsDirectory.h"
#include "b_io.h"
#include "fsInit.c"

/******************************************************************************
 *                        -----parses file path-----
 * parsing the file path with the token "/" to denote when the path begins and
 * ends.
 *****************************************************************************/
char *parsePath(char *path)
{
    char *filePathArr;
    // savePtr for strtok_r
    char *savePtr;
    // splits up by path /
    char *token = strtok_r(path, '/', &savePtr);
    while (token != NULL)
    {
        strcpy(filePathArr, token);
        token = strtok_r(NULL, "/", &savePtr)
    }
    // return array of path
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
 *               -----Finds entry based on parsed path-----
 * Takes char array with parsed path as parameter and length of array.
 *  Reads through parsed path one name at a time. Starts by opening root directory. 
 * Returns a pointer to an array of entries.
 *****************************************************************************/

entryStruct getEntryFromPath(char *arr, int arrLength){

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
            return -1;
        }
    }

    return entry_p;
}



/******************************************************************************
 *                   -----opens a directory-----
 * 
 *****************************************************************************/
fdDir *fs_opendir(const char *name)
{
    int fd;
    // call b_open
    fd = b_open(name, O_RDWR);
    // set all corresponding values of b_open to fdDir

    fdDir dir;

    for (int i = 0; i < entriesLength; i++)
    {
        if (strcmp(entries[i].path, name))
        {
            dir->name = name;
            // find rec length
            // find entry position in directory
            // find         }
        } // (length,LBA of directory direntry position, starting LBA of directory, name? )

/******************************************************************************
 *                      -----read a directory-----
 * This function returns the next directory entry of the current directory entry.
 * 
 *****************************************************************************/
        struct fs_diriteminfo *fs_readdir(fdDir * dirp)
        {
            // fs_diriteminfo in mfs.h
            DIR *dir;
            dir = (DIR *)dirp;
            /** See: https://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html */
            struct dirent *di;
            /** readdir() returns a pointer to a dirent structure showing the next dir entry
     *  dirp is pointing to. Returns null if there's an error or at the end of dir
     *  stream.
     */
            di = readdir(dir);
            return ((struct mfs_diriteminfo *)di);
        }

        /******************************************************************************
 *                      -----close a directory-----
 * 
 *****************************************************************************/
        int fs_closedir(fdDir * dirp)
        {
            // fdDir in mfs.h
            DIR *dir;
            dir = (DIR *)dirp;
            /** Closedir() takes a pointer to a DIR referring to the open directory
     *  stream desired to be closed. A return value of 0 is successful, -1 is
     *  not.
     */
            return (closedir(dir));
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

            entry_p = getEntryFromPath(parsedPath,pathLength);

    if(strcmp(entry_p->type, "f"){
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
            entryStruct *entry_p;
            // look for path in entries
            for (int i = 0; i < entriesLength; i++)
            {
                if (strcmp(entries[i].path, path))
                {
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
            struct stat *pstat;
            pstat = (struct stat *)buf;
            // use this to set all values of fs_stat struct
            /** stat() displays file or file system status; returns 0 if successful. */
            /** See: https://pubs.opengroup.org/onlinepubs/009696799/functions/stat.html */
            return (stat(path, pstat));
        }
