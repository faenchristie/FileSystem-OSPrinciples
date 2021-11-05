/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: fsDirectoryOps.c
*
* Description: Represents entries whether they be files
or directories.
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

/******************************************************************************
 * -----opens a directory-----
 * 
 *****************************************************************************/
fdDir *fs_opendir(const char *name)
{
    // call b_open

    // set all corresponding values of b_open to fdDir

    // (length, direntry position, starting LBA of directory, name? )

    // return fdDir

    return 0;
}

/******************************************************************************
 * -----read a directory-----
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
 * -----close a directory-----
 * 
 *****************************************************************************/
int fs_closedir(fdDir *dirp)
{
    // b_close ?
    return 0;
}

/******************************************************************************
 * -----check if it is a file or not-----
 * return 1 if file, 0 otherwise
 *****************************************************************************/
int fs_isFile(char *path)
{
    // parse path

    // look for path in entries

    // if path doesn't exist, give error, return 0

    // if entry of path is found, check value to see if file

    // if it is file, return 1

    return 0;
}

/******************************************************************************
 * -----check if it is a directory or not-----
 * return 1 if directory, 0 otherwise
 * 
 *****************************************************************************/
int fs_isDir(char *path)
{

    // parse path

    // look for path in entries

    // if path doesn't exist, give error, return 0

    // if path exists, check if its value is a directory

    // if it is a directory, return 1

    return 0;
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