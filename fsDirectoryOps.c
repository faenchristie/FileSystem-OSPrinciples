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
fdDir *fs_opendir(const char *name) {
    return 0;
}

/******************************************************************************
 * -----read a directory-----
 * 
 *****************************************************************************/
struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    return 0;
}

/******************************************************************************
 * -----close a directory-----
 * 
 *****************************************************************************/
int fs_closedir(fdDir *dirp) {
    return 0;
}

/******************************************************************************
 * -----check if it is a file or not-----
 * return 1 if file, 0 otherwise
 *****************************************************************************/
int fs_isFile(char *path) {
    return 0;
}

/******************************************************************************
 * -----check if it is a directory or not-----
 * return 1 if directory, 0 otherwise
 * 
 *****************************************************************************/
int fs_isDir(char *path) {
    return 0;
}

/******************************************************************************
 * 
 * 
 *****************************************************************************/
int fs_stat(const char *path, struct fs_stat *buf) {
    return 0;
}