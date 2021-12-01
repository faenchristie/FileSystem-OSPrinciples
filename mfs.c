/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: mfs.c
*
* Description: File contains functions for directory operations 
* including read, close, open, isfile, isDir, stat. 
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
 * GLOBAL VARIABLES HERE
 *****************************************************************************/
entryStruct *currentEntry = NULL; // keeps track of entries we are looking for
entryStruct *parentEntry = NULL;
struct fs_diriteminfo * directoryEntry = NULL; // directoryEntry info
int entrySize = sizeof(entryStruct) * DIRENTRIES; // general entrySize malloc

/******************************************************************************
 *                        -----parses file path-----
 * 
 * parsing the file path with the token "/" to denote when the path begins and
 * ends.
 * 
 * get the length of the parsedPath and store it in a variable
 * loop through the variable and parse
 * save into new parsedpath global variable
 *****************************************************************************/
 void parsePath(char *path) {
     // get length of parsedPath, traverse through, nullify
    int arrLength = getArrLength(parsedPath);

    for(int i = 0; i < arrLength; i++) {
        parsedPath[i] = "\0";
    }
    char *savePtr;
    char *delim = "/";
    char *token = strtok_r(path, delim, &savePtr);

    int i = 0;
    // save into new parsedPath global variable
    while(token != NULL){
        parsedPath[i] = token;
        i++;
        token = strtok_r(NULL, delim, &savePtr);
    }
}

/******************************************************************************
 *                       -----returns length of arr-----
 * 
 * Takes a char array as input and returns the length of it. This will be
 * used to dissect the parent path.
 * 
 * create a variable to track the count
 * while loop under the condition that the array count is not null, update the
 * count.
 * 
 * return count
 *****************************************************************************/
int getArrLength(char *arr[]) {

    int count = 0;

    while (arr[count] != NULL) { 
        count++;
    }

    return count;
}

/******************************************************************************
 *                 -----gets parent path based on path-----
 * 
 * Takes a path parameter of a path. Loops through path by length-1, effectively
 * removing the last part of the path (the child). This parent path can then be
 * used in conjunction with getEntryFromPath to return the entry.
 * 
 *****************************************************************************/
void getParentPath() {
    // nullify past parent path
    int arrLength = getArrLength(parentPath);

    for(int i = 0; i < arrLength; i++){
        parentPath[i] = "\0";
    }
     // get length of original path
    int pathLength = getArrLength(parsedPath);
     // copies in path until the end, removing child
    for(int i = 0; i < pathLength-1; i++) {
        parentPath[i] = parsedPath[i];
    }
}

/******************************************************************************
 *               -----Finds entry based on parsed path-----
 * 
 * Takes char array with parsed path as parameter and length of array.
 * Reads through parsed path one name at a time. Starts by opening root directory. 
 * Returns a pointer to an array of entries.
 *****************************************************************************/
void getEntryFromPath(char *arr[], int arrLength) {

    // check if currentEntry needs to be malloc'd
    if(currentEntry == NULL) {
    currentEntry = malloc(entrySize);
    }
    // read in root directory into entry_p using info from vcbb 
    LBAread(currentEntry, vcb_p->rootDirBlocks, vcb_p->rootDir);
    // return root 
    if(strlen((char*)arr) == 0 || strcmp(arr[0], "") == 0) {
        return;
    }

    // "boolean" that checks if parent entry was found
    int found = 0;

    // first for loop loops through each segment of the path
    for(int i = 0; i < arrLength; i++) {
        // inner for loop loops through each entry of the directory
        for(int j = 2; j < DIRENTRIES; j++) {
            // compares the path name to the  name of each directory entry
            if(strcmp(currentEntry[j].name, arr[i]) == 0) {
                // read next directory into tempEntry if matches
                int blockCount = currentEntry[j].blockCount;
                int blockLocation = currentEntry[j].blockLocation;
                LBAread(currentEntry, currentEntry[j].blockCount, currentEntry[j].blockLocation);
                // directory found; move onto next
                found = 1;
                break;
            } else {
                found = 0;
            }
        }
        // once done looping through, check if next entry was found
        if(found == 0) {
            // no entry of this name was found, hence it is NULL.
            currentEntry = NULL;
            printf("DIRECTORY NOT FOUND\n");
            return;
        }
    }
    return;
}

/******************************************************************************
 *                         -----opens a directory-----
 * 
 * This function opens a directory
 * 
 * we first need to call b_open
 * then, if the file descriptor is less than 0 throw an error statement
 * next, malloc the directory to be the size of fdDir and then parse the path
 * get the global variable from the function call getEntryFromPath
 * if the directory waasnt found @return NULL
 * next we have to assign the values of the dir then free
 * 
 * @return dir
 *****************************************************************************/
fdDir *fs_opendir(const char *name) {
    int fd;
    struct dirent *openDir;
    char *filename = (char*)name;

    // open entry to get fd 
    fd = b_open(filename, O_RDWR);

    if(fd < 0){
        printf("Could not open %s directory.\n", name);
        return NULL;
    }
    // fdDir struct to keep track of entry information
    fdDir *dir; 
    dir = malloc(sizeof(fdDir));
    parsePath(filename); // parse path to global variable parsePath
    int pathLength = getArrLength(parsedPath);

    getEntryFromPath(parsedPath, pathLength); 

    if(currentEntry == NULL){
        printf("No such directory %s found.\n",name);
        return NULL;
    }
    // starting block of where directory is located
    dir->directoryStartLocation = currentEntry[0].blockLocation;
    dir->directoryBlockAmount = currentEntry[0].blockCount;
    dir->childrenAmount = currentEntry[0].childrenAmount;
    dir->readDirIndex = 0;
    dir->readDirRead = 0;
    //dir->d_reclen = 

    free(currentEntry);
    currentEntry = NULL;
    
    return dir;
}

/******************************************************************************
 *                      -----read a directory-----
 * 
 * This function returns the next directory entry of the current directory 
 * entry.
 * 
 * malloc if needed and check to make sure we went through all children
 * then read the entry into memory with LBAread call
 * and iterate through the entries with a for loop to begin assigning
 * finally increment and free
 * 
 * ISSUE: return is giving warning
 * 
 * @return directoryEntry
 *****************************************************************************/
struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    
    if(directoryEntry == NULL) {
    directoryEntry = malloc(sizeof(struct fs_diriteminfo));
    }

    // check if we went through all children
    if(dirp->readDirRead == dirp->childrenAmount) {
        // index of children starts at 2 , 0 and 1 are . and ..
        dirp->readDirIndex = 0;
        dirp->readDirRead = 0;
        // finally free directoryEntry
        free(directoryEntry);
        return NULL;
    }
    // use to find entry
    entryStruct *entry;
    entry = malloc(entrySize);
    // start at previous index + 1 to start
    int start = dirp->readDirIndex + 1;

    // read entry into memory
    LBAread(entry, dirp->directoryBlockAmount, dirp->directoryStartLocation);

    // cycle through entries 
    for(int i = start; i < DIRENTRIES; i++) {
        // if entry is not unused, 0 means undefined entry
        if(entry[i].type != 0) {
            // assign type
            if(entry[i].type == 1) {
                strcpy(directoryEntry->fileType, "Directory");
            }
            if(entry[i].type == 2) {
                strcpy(directoryEntry->fileType, "File");
            }
            // assign name
            strcpy(directoryEntry->d_name, entry[i].name);

            // index of found entry will be used for next read
            dirp->readDirIndex = i;
            break;
        }
    }
    // increment amount of children read
    dirp->readDirRead++;

    free(entry);

    // check out this return type later, giving warning
    return directoryEntry;
}

/******************************************************************************
 *                      -----close a directory-----
 * 
 * This function will close a directory
 * First we need to verify if dirp is an open directory by using a simple if
 * statement, then we prompt the user that the directory is closing
 * finally we free the dirp. 
 * 
 * ISSUE: This crashes program when the free(dirp) is called
 * 
 * According to manpage, the value of dirp should be insaccessible upon the 
 * functions return.
 * 
 * @return 0
 *****************************************************************************/
int fs_closedir(fdDir *dirp) {

    if(dirp==NULL){
        printf("Directory not open.");
        return -1;
    }

    //printf("Closing directory...\n");

    // free(): invalid pointer
    //free(dirp);

    printf("\n");
    return 0;
}

/******************************************************************************
 *                  -----check if it is a file or not-----
 * 
 * First, we created a variable for the length of our array in order to
 * traverse using a for loop. Next, inside the For loop there will need to be
 * a comparison for the path. Outside the for loop, we have to make sure
 * the types line up from the entries, to do this we compare with int 2
 * which refers to the fact that it is a file. Returning 1 if it is, or 0
 * if it is not a file.
 *****************************************************************************/
int fs_isFile(char *path) {
    
    parsePath(path); // returns to global variable parsedPath

    // get length of parsed path (used for next function)
    int pathLength = getArrLength(parsedPath);

    // returns to global vatiable currentEntry
    getEntryFromPath(parsedPath, pathLength);

    // type 2 means file in our system, return true
    if(currentEntry[0].type == 2){
        return 1;
    }else{
        return 0;
    }
}

/******************************************************************************
 *                 -----check if it is a directory or not-----   
 *                
 * First, we created a variable for the length of our array in order to
 * traverse using a for loop. Next, inside the For loop there will need to be
 * a comparison for the path. Outside the for loop, we have to make sure
 * the types line up from the entries, to do this we compare with int 1
 * which refers to the fact that it is a directory. Returning 1 if it is, or 0
 * if it is not a directory. if the type is 1 @return 1, else @return 0.
 *****************************************************************************/
int fs_isDir(char *path) {
    // returns to global variable parsedPath
    parsePath(path);

    // get length of parsed path (used for next function)
    int pathLength = getArrLength(parsedPath);

    // returns to global vatiable currentEntry
    getEntryFromPath(parsedPath,pathLength);

    // if it is a directory, return successful
    if(currentEntry[0].type == 1){
        // make sure to check if currentEntry needs to be freed
        if(currentEntry!=NULL){
        free(currentEntry);
        currentEntry = NULL;
        }
        return 1;
    }else{
        // make sure to check if currentEntry needs to be freed
        if(currentEntry!=NULL){
        free(currentEntry);
        currentEntry = NULL;
        }
        // return unsucsseful if not directory
        return 0;
    }
}

/******************************************************************************
 *                 ----information about the filesystem-----
 * 
 * This function returns information about our filesystem from the struct 
 * that was created in mfs.h
 * 
 * we first need to check if the entry is a directory or a file, with a simple
 * if statement
 * next, we get the length of the parsed path and set the entry pointer to hold
 * the information from the getEntryFromPath function call
 * Next, we print out all the information of our filesystem to the terminal
 *
 * return 0 if successful.
 * 
 * @return 0
 *****************************************************************************/
int fs_stat(const char *path, struct fs_stat *buf) {
    // find entry that matches path
    entryStruct * entry_p;
    //char *parsedPath = parsedPath(path);
    
    // to fix const char error
    char * filepath = (char *)path;
    parsePath(filepath);

    // Check if entry is a directory or file
    if(fs_isDir(filepath) != 1 || fs_isFile(filepath) != 1){
        printf("%s is not a valid entry.\n", path);
        return -1;
    }

    int pathLength = getArrLength(parsedPath);
    getEntryFromPath(parsedPath, pathLength);

    // print out entry's information
    printf("Total Size: %ld\n", (buf->st_size = currentEntry[0].size));
    printf("Number of blocks: %ld\n", (buf->st_blocks = currentEntry[0].blockCount));
    printf("Blocksize: %ld\n", (buf->st_blksize = vcb_p->blockSize));
    
    free(entry_p);
    return 0;
}


/******************************************************************************
 *                             -----removes a file-----
 * 
 * This function will delete a file
 * we created three variables and set them all to 0.
 * next, we need to concat the filename and cwd
 * Parse the path of the current working directory and get the length of the
 * parsed path, then get the entry from the path. 
 * if the current entry is null, throw and error, otherwise get the parentEntry
 * call an LBARead
 * and loop through our directory entries using a string comparison then set
 * the freespace map to free
 * 
 * @return 0
 *****************************************************************************/
int fs_delete(char *filename) {
    // used to find location
    int blocksUsed = 0;
    int blockStart = 0;
    // used to return error if file doesn't exist
    int entryNo = 0;

    // combine filename and working directory?
    char *cwd; // = current working directory variable
    strcat(cwd,"/");
    strcat(cwd,filename);
    
    // use combined path to get entry
    parsePath(cwd);
    int pathLength = getArrLength(parsedPath);
    getEntryFromPath(parsedPath,pathLength);

    if (currentEntry == NULL) {
        printf("ERROR: FILE NOT FOUND\n");
        return 0;
    } else { // if entry is valid
        entryStruct *parentEntry;
        parentEntry = malloc(entrySize);
        int parentLocation = currentEntry[0].parent;
        int parentCount = currentEntry[0].parentCount;

        // read parent into memory using variables stored in dir
        LBAread(parentEntry, parentCount, parentLocation);

        // find child in parent entry to remove from its list
        for(int i = 0; i < DIRENTRIES; i++){
        // if name matches, entry found
            if(strcmp(parentEntry[i].name, parsedPath[pathLength-1]) ==0){
                parentEntry[i].name[0] = '\0';
                parentEntry[i].blockLocation = 0;
                parentEntry[i].blockCount = 0;
                parentEntry[i].type = 0;
                parentEntry[i].parent = 0;
                parentEntry[i].parentCount = 0;
                parentEntry[i].childrenAmount= 0;
                // decrement "." of parent entry
                parentEntry[0].childrenAmount--;
            }
        }
        for (int i = blockStart; i < blocksUsed + 1; i++) {
            freeMap[i] = 0;
        }
        printf("FILE DELETED\n");
        free(parentEntry);
    }
    free(currentEntry);
    return 0;
}

/******************************************************************************
 *                     -----creates a new directory-----
 * 
 * This function makes a new directory
 * 
 * first malloc space for the entry
 * get the pathname, and length
 * calculate size for blocks needed and find free block space that will cover
 * required space in bitmap
 * get the parentpath and the parent entry then start assigning values
 * We then need to update the bitmap to signify the used space and then finally
 * start to free
 * 
 * @return 0
 *****************************************************************************/
int fs_mkdir(const char *pathname, mode_t mode) {
    char * path = (char *)pathname;

    entryStruct * entry_p;
    // malloc space for entry_p
    entry_p = malloc(entrySize);
    
    // get pathname, pathname length
    parsePath(path);
    int pathLength;
    pathLength = getArrLength(parsedPath);

    // blocks needed for this entry based on size
    int blocksNeeded = ((entrySize + BLOCKSIZE - 1) / BLOCKSIZE);
    int blockNo = 0;

    // find free block space that will cover the amount needed in bitmap
    blockNo = findFreeBlocks(blocksNeeded);

    if (blockNo==0) {
        printf("ERROR: NOT ENOUGH MEMORY\n");
        return -1;
    }

    // get parent path
    getParentPath(path);
    int parentPathLength = getArrLength(parentPath);

    // get parent entry
    getEntryFromPath(parentPath,parentPathLength);
    if(currentEntry==NULL){
        printf("DIRECTORY NOT FOUND\n");
        return -1;
    }
    // assign values to parent entry
    // increment children amount of parent
    currentEntry[0].childrenAmount++;

    // first entry is just a pointer to itself
    entry_p[0].parent = currentEntry[0].blockLocation;
    entry_p[0].blockLocation = blockNo;
    entry_p[0].blockCount = blocksNeeded;
    strcpy(entry_p[0].name, ".");
    entry_p[0].childrenAmount = 0;
    entry_p[0].type = 1;

    // next entry is pointer to parent
    entry_p[1].parent = currentEntry[0].parent;
    entry_p[1].blockLocation = currentEntry[0].blockLocation;
    entry_p[1].blockCount = currentEntry[0].blockCount;
    entry_p[1].childrenAmount = currentEntry[0].childrenAmount;
    strcpy(entry_p[1].name,"..");
    entry_p[1].type = 1;

    // initialize other values as undefined entry
    for(int i = 2; i < DIRENTRIES - 2; i++) {
        entry_p[i].type = 0;
    }
    for(int i = 2; i <  DIRENTRIES - 1; i++) {
        // empty entry, can be used
        if(currentEntry[i].type==0){
            currentEntry[i].blockLocation = blockNo;
            currentEntry[i].blockCount = blocksNeeded;
            currentEntry[i].type = 1;
            currentEntry[i].childrenAmount = 0;
            // increment amount of children of this dir
            currentEntry[0].childrenAmount++;

            // set name to the last item in the parsed pathh arr
            strcpy(currentEntry[i].name,parsedPath[pathLength-1]);
            LBAwrite(currentEntry,currentEntry[0].blockCount,currentEntry[0].blockLocation);
            break;
        }
    }

    // write new entry to memory
    LBAwrite(entry_p, blocksNeeded, blockNo);

    // update bitmap to signify used space
    for (int i = blockNo; i < blockNo + blocksNeeded; i++) {
        freeMap[i] = 1;entryStruct *currentEntry;
    }

    // free malloc'd entry now that it is written in memory
    free(currentEntry);
    currentEntry = NULL;
    free(entry_p);
    entry_p = NULL;

    printf("Directory '%s' successfully made\n",pathname);

    return 0;
}

/******************************************************************************
 *                       -----remove a directory-----
 * 
 * This function will remove a directory given the pathname as an argument
 * 
 * first, we created 3 variables, blockNo, totalBlocks, and found.
 * Then we must next parse the path of the argument, in this case pathname
 * then we create another variable which holds the length of the parsed path
 * We then call the getentryFromPath which saves into a global variable called
 * currentEntry.
 * if the current entry is not null, found = 1, otherwise throw error printf
 * Next, we need to check if directory has children or not by running an if 
 * statement
 * We then create a few variables that are necessary for finding parent
 * call LBAread, and then loop through our directory entries using a strcmp
 * set to 0 if found, then finally mark our bitmap space to free
 * 
 * @return 0
 *****************************************************************************/
int fs_rmdir(const char *pathname) {
    
    int blockNo; // block that entry starts at
    int totalBlocks; // total blocks it uses
    //int entryNo;
    int found = 0; // bool to check if path exists

    // cast
    char * path = (char *)pathname;

    // parse file path to global variable parsedPath
    parsePath(path);

    // get length of array (used for getentry function)
    int pathLength = getArrLength(parsedPath);

    // get entry save in global variable currentEntry
    getEntryFromPath(parsedPath, pathLength);

    if(currentEntry != NULL){
        found = 1;
    }
    // if not found
    if (!found) {
        printf("DIRECTORY DOES NOT EXIST\n");
        // free currentEntry
        free(currentEntry);
        currentEntry = NULL;
        return -1;
    }
    // check if directory has children or not, return error if does
    if(currentEntry[0].childrenAmount != 0){
        // free currentEntry
        free(currentEntry);
        currentEntry = NULL;
        printf("CANNOT DELETE DIRECTORY WITH ENTRIES");
        return -1;
    }
    // variables needed to find parent entry
    if(parentEntry==NULL){
    printf("parentEntry==NULL\n");
    parentEntry = malloc(entrySize);
    }
    // location needed to read into memory
    int parentLocation = currentEntry[0].parent;
    int parentCount = currentEntry[0].parentCount;
    // read parent into memory using variables stored in dir
    LBAread(parentEntry,parentCount,parentLocation);

    // find child in parent entry to remove from its list
    for(int i = 0; i < DIRENTRIES; i++) {
        // if name matches, entry found
        if(strcmp(parentEntry[i].name,parsedPath[pathLength-1]) == 0){
            parentEntry[i].name[0] = '\0';
            parentEntry[i].blockLocation = 0;
            parentEntry[i].blockCount = 0;
            parentEntry[i].type = 0;
            parentEntry[i].parent = 0;
            parentEntry[i].parentCount = 0;
            parentEntry[i].childrenAmount= 0;
            // decrement "." of parent entry
            parentEntry[0].childrenAmount--;
        }
    }

    printf("DIRECTORY DELETED\n");

    // mark bitmap space to 0 to signify space is free
    for (int i = blockNo; i < totalBlocks + 1; i++) {
        freeMap[i] = 0;
    }

    // free malloc'd space, NULLify for reference
    free(parentEntry);
    parentEntry = NULL;
    free(currentEntry);
    currentEntry == NULL;

    return 0;
}

/******************************************************************************
 *              -----set the current working directory-----
 * 
 * This function will set the current working dirrectory
 * First, get teh len of the current directory path to see if it is bigger 
 * than the size, if it is, then throw an error and @return 0
 * then, use a simple strcpy to copy the current path to the buffer
 * 
 * @return buf
 *****************************************************************************/
char *fs_getcwd(char *buf, size_t size) {
    if (strlen(currentDirectoryPath) > size) {
        printf("ERROR PATH TOO LONG\n");
        return 0;
    }

    strcpy(buf, currentDirectoryPath);

    return buf;
}

/******************************************************************************
 *              ------Set Current Working Directory-----
 * 
 * This function will set the current working directory.
 * first, creating a parsedPath variable for the buffer we are parsing
 * then check the validity of the path with an if statement
 * search for the directory based on the path
 * clear the old path
 * then, set the new current working directory by copying parsed path to the 
 * currentDirectoryPathArray, we used a sprintf to format the output to string
 * pointed to
 * 
 * @return 0
 *****************************************************************************/
int fs_setcwd(char *buf) {
    parsePath(buf);

    // Check validity of path
    if(fs_isDir(buf) != 1){
        printf("%s is not a valid directory.\n", buf);
        return -1;
    }

    int pathLength = getArrLength(parsedPath);
    // clear old path
    currentDirectoryPath[0] = '\0';

    // copy in new path
    strcpy(currentDirectoryPath,buf);

    printf("Setting cwd to %s\n", currentDirectoryPath);

    return 0;
}