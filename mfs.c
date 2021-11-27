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
 * GLOBAL VARIABLES HERE
 *****************************************************************************/


/******************************************************************************
 *                        -----parses file path-----
 * 
 * parsing the file path with the token "/" to denote when the path begins and
 * ends.
 *****************************************************************************/
 void parsePath(char *path) {
     // get length of parsedPath, traverse through, nullify
    int arrLength = getArrLength(parsedPath);
    printf("arrlength, %i\n", arrLength);
    for(int i = 0; i < arrLength; i++){
        parsedPath[i] = "\0";
    }
    printf("parse path error 1\n");
    char *savePtr;
    printf("parse path error 2\n");
    char *delim = "/";
    char *token = strtok_r(path, delim, &savePtr);
    printf("parse path error 3\n");
    int i = 0;
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
 * used to dissect the parent path.
 * 
 * create a variable to track the count
 * while loop under the condition that the array count is not null, update the
 * count.
 * 
 * @return count
 *****************************************************************************/
int getArrLength(char *arr[]) {
    printf("GET ARR LENGTH FUNCTION\n");

    int count = 0;
    // will go until null is found
    while (arr[count] != NULL) { //removed (arr[count] != ('\0')||) 
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
    for(int i = 0; i < arrLength; i++){
        parentPath[i] = "\0";
    }
     // get length of original path
    int pathLength = getArrLength(parsedPath);
    printf("path length: %i\n", pathLength);
     // copies in path until the end, removing child
    for(int i = 0; i < pathLength-1; i++){
        printf("Path[i], %s\n", parsedPath[i]);
        parentPath[i] = parsedPath[i];
    }
    int parentPathLength = getArrLength(parentPath);
    printf("Parent Path length: %i\n", parentPathLength);
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
    LBAread(tempEntry, vcb_p->rootDirBlocks, vcb_p->rootDir);
    printf("GETENTRY ERROR 3\n");
    // if arr length = 0, we are simply returning root.
    
    if(arrLength == 0){
        currentEntry = tempEntry;
        //free(tempEntry);
        return;
    }

    // "boolean" that checks if parent entry was found
    int found = 0;

    printf("GETENTRY ERROR 4\n");
    // first for loop loops through each segment of the path
    for(int i = 0; i <= arrLength; i++){
        printf("GETENTRY ERROR 5\n");
        // inner for loop loops through each entry of the directory
        for(int j = 2; j < DIRENTRIES; j++){
            printf("GETENTRY ERROR 6\n");
            // ******************** //
            // NOTE: Should I check if value is null to avoid segfault?
            // ******************** //
            printf("tempEntry name: %s\n", tempEntry[j].name);
            printf("arr[0]: %s\n", arr[0]);
            // compares the path name to the  name of each directory entry
            if(strcmp(tempEntry[j].name,arr[i])==0){
                printf("GETENTRY ERROR 7\n");
                printf("currentEntry block count: %i\n",currentEntry[j].blockCount);
                printf("currentEntry block location: %i\n",currentEntry[j].blockLocation);
                // read next directory into tempEntry if matches
                LBAread(tempEntry, currentEntry[j].blockCount, currentEntry[j].blockLocation);
                printf("GETENTRY ERROR 8\n");
                // move onto next entry
                currentEntry = tempEntry;
                printf("currentEntry block count: %i\n",currentEntry[j].blockCount);
                printf("currentEntry block location: %i\n",currentEntry[j].blockLocation);
                printf("currentEntry type: %i\n",currentEntry[j].type);
                printf("GETENTRY ERROR 9\n");
        
                break;
            } else{
                // make entry_p null if not found. This will be used to check
                // if the directory was found. 
                printf("GETENTRY ERROR 10\n");
                currentEntry = NULL;
                printf("GETENTRY ERROR 11\n");
            }
            
        }
        // once done looping through, check if next entry was found
        if(currentEntry == NULL){
            // no entry of this name was found, hence it is NULL.
            // print error, return.
            printf("DIRECTORY NOT FOUND\n");
            return;
        }
        if(found = 1){
            break;
        }
    }
    printf("Current entry block location: %i\n", currentEntry[0].blockLocation);
    printf("GETENTRY ERROR 11\n");
    // free the temp entry
    //free(tempEntry);
}




/******************************************************************************
 *                         -----opens a directory-----
 * *****TODO****
 * This function opens a directory
 * 
 * we first need to call b_open
 * then, if the file descriptor is less than 0 throw an error statement
 * next, malloc the directory to be the size of fdDir and then parse the path
 * get the global variable from the function call getEntryFromPath
 * if the directory waasnt found @return NULL
 * next we have to assign the values of the dir
 * 
 * @return dir
 *****************************************************************************/
fdDir *fs_opendir(const char *name) {
    printf("OPENDIR FUNCTION\n");
    int fd;
    struct dirent *openDir;
    // added bc const char error?
    char *filename = (char*)name;
    /* 
    * warning: passing argument 1 of ‘b_open’ discards ‘const’ qualifier from pointer target type [-Wdiscarded-qualifiers]
    * mfs.c:33:7: note: expected ‘char *’ but argument is of type ‘const char *’
    */
    fd = b_open(filename, O_RDWR);

    if(fd < 0){
        printf("Could not open %s directory.\n", name);
        return NULL;
    }

    fdDir *dir; 
    dir = malloc(sizeof(fdDir));

    parsePath(filename); // parse path to global variable parsePath

    int pathLength = getArrLength(parsedPath);

    getEntryFromPath(parsedPath, pathLength);

    printf("OPENDIR ERROR 1\n");


    if(currentEntry == NULL){
        printf("No such directory %s found.\n",name);
        return NULL;
    }

    printf("OPENDIR ERROR 2\n");
    //printf("Current entry block location: %i\n",currentEntry[0].blockLocation);

    // should this be 2? index 2 would be the first child entry
    // of the directory.
    dir->dirEntryPosition = 2; 
    printf("OPENDIR ERROR 3\n");
    // starting block of where directory is located
    printf("Current entry block location: %i\n",currentEntry[0].blockLocation);
    dir->directoryStartLocation = currentEntry[0].blockLocation;
    printf("OPENDIR ERROR 4\n");
    dir->directoryBlockAmount = currentEntry[0].blockCount;
    printf("OPENDIR ERROR 5\n");
    // add logic for counting children if possible
    dir->childrenAmount = currentEntry[0].childrenAmount;
    printf("OPENDIR ERROR 6\n");
    // figure out how to assign this
    //dir->d_reclen = 

    return dir;
}
/******************************************************************************
 *                      -----read a directory-----
 * 
 * This function returns the next directory entry of the current directory 
 * entry.
 * Created a variable to keep track of the children count, and another variable
 * that is the directoryEntry.
 * if statement to see if the index is the same as the amount of children that
 * is in the struct, reset the counter if it is
 * update the index
 * 
 * @return address of directoryEntry
 *****************************************************************************/
struct fs_diriteminfo *fs_readdir(fdDir *dirp) {

    int readdirIndex = 0; //Keeps track of children count
    struct fs_diriteminfo directoryEntry;

    if(readdirIndex == dirp->childrenAmount){
        readdirIndex = 0;
        printf("Could not read.\n");
        return NULL;
    }

    readdirIndex++;

    // check out this return type later, giving warning
    return &directoryEntry;
}

/******************************************************************************
 *                      -----close a directory-----
 * 
 * This function will close a directory
 * First we need to verify if dirp is an open directory by using a simple if
 * statement, then we prompt the user that the directory is closing
 * finally we free the dirp.
 * 
 * According to manpage, the value of dirp should be insaccessible upon the 
 * functions return.
 * 
 * @return 0
 *****************************************************************************/
int fs_closedir(fdDir *dirp) {
    //Verify if dirp is an open directory
    if(dirp = fs_opendir(dirp->name) == NULL){
        printf("%s is not an opened directory.\n", dirp->name);
        return -1;
    }

    printf("Closing directory...\n");
    free(dirp);

    return 0;
}

/******************************************************************************
 *                  -----check if it is a file or not-----
 * 
 * First, we created a variable for the length of our array in order to
 * traverse using a for loop. Next, inside the For loop there will need to be
 * a string comparison for the path. Outside the for loop, we have to make sure
 * the types line up from the entries, to do this we compare with a char D
 * which refers to the fact that it is a file. Returning 1 if it is, or 0
 * if it is not a file.
 *****************************************************************************/
int fs_isFile(char *path) {
    printf("IS FILE FUNCTION\n");
    
    parsePath(path); // returns to global variable parsedPath

    // get length of parsed path (used for next function)
    int pathLength = getArrLength(parsedPath);

    // returns to global vatiable currentEntry
    getEntryFromPath(parsedPath,pathLength);

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
 * a string comparison for the path. Outside the for loop, we have to make sure
 * the types line up from the entries, to do this we compare with a char D
 * which refers to the fact that it is a directory. Returning 1 if it is, or 0
 * if it is not a directory.
 *****************************************************************************/
int fs_isDir(char *path) {
    printf("IS DIR FUNCTION\n");
    // returns to global variable parsedPath
    parsePath(path);

    // get length of parsed path (used for next function)
    int pathLength = getArrLength(parsedPath);

    // returns to global vatiable currentEntry
    getEntryFromPath(parsedPath,pathLength);
    printf("currentEntry.type %i\n",currentEntry[0].type);
    if(currentEntry[0].type == 1){
        return 1;
    }else{
        return 0;
    }
    free(currentEntry);
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
    // ****** COMMENTED OUT BC NOT VALID TYPE *****
    //printf("Created: %d\n", (buf->st_createtime = entry_p->st_createtime));
    //printf("Access time: %d\n", (buf->st_accesstime = entry_p->st_accesstime));
    //printf("Last modification: %d\n", (buf->st_modtime = entry_p->st_modtime));

    // return 0 if successful.
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
        int entrySize = sizeof(entryStruct) * DIRENTRIES;
        parentEntry = malloc(entrySize);
        int parentLocation = currentEntry[0].parent;
        int parentCount = currentEntry[0].parentCount;

        // read parent into memory using variables stored in dir
        LBAread(parentEntry,parentCount,parentLocation);

        // find child in parent entry to remove from its list
        for(int i = 0; i < DIRENTRIES; i++){
        // if name matches, entry found
            if(strcmp(parentEntry[i].name,parsedPath[pathLength-1])==0){
                parentEntry[i].name[0] = '\0';
                parentEntry[i].blockLocation = 0;
                parentEntry[i].blockCount = 0;
                parentEntry[i].type = 0;
                parentEntry[i].parent = 0;
                parentEntry[i].parentCount = 0;
                parentEntry[i].childrenAmount= 0;
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
 * ****TODO**** comments, testing
 *****************************************************************************/
int fs_mkdir(const char *pathname, mode_t mode) {
    // to fix const char error
    char * path = (char *)pathname;
    // malloc space needed: size of struct * average entries for directory
    int entrySize = sizeof(entryStruct) * DIRENTRIES;
    entryStruct * entry_p;
    // malloc space for entry_p
    entry_p = malloc(entrySize);
    
    // get pathname, pathname length
    parsePath(path);
    int pathLength;
    pathLength = getArrLength(parsedPath);

    printf("Path 0 = %s\n",parsedPath[0]);

    printf("PathLength %i",getArrLength(parsedPath));

    int blocksNeeded = ((entrySize + BLOCKSIZE - 1) / BLOCKSIZE);
    int blockNo = 0;

    printf("blocksNeeded %d\n",blocksNeeded);
    printf("blockNo %d\n",blockNo);

    // find free block space that will cover the amount needed in bitmap

    blockNo = findFreeBlocks(blocksNeeded);

    printf("blockNo %d\n",blockNo);
    
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
    // get parent info, assign values to entry_p 
    printf("parent location: %i\n",currentEntry[0].blockLocation);
    printf("parent count: %i\n",currentEntry[0].blockCount);
    printf("parent name: %s\n",currentEntry[0].name);

    // increment children amount of parent
    currentEntry[0].childrenAmount++;

    // first entry is just a pointer to itself
    entry_p[0].parent = currentEntry[0].blockLocation;
    entry_p[0].blockLocation = blockNo;
    entry_p[0].blockCount = blocksNeeded;
    strcpy(entry_p[0].name, ".");
    entry_p[0].childrenAmount = 0;
    entry_p[0].type = 2;

    // next entry is pointer to parent
    entry_p[1].parent = currentEntry[0].parent;
    entry_p[1].blockLocation = currentEntry[0].blockLocation;
    entry_p[1].blockCount = currentEntry[0].blockCount;
    entry_p[1].childrenAmount = currentEntry[0].childrenAmount;
    strcpy(entry_p[1].name,"..");
    entry_p[1].type = 2;

    // initialize other values as undefined entry
    for(int i = 2; i < DIRENTRIES-2; i++) {
        entry_p[i].type = 0;
    }
    printf("PARSED PATH: %s\n",parsedPath[pathLength-1]);
    for(int i=2; i<DIRENTRIES-1; i++){
        printf("currentEntry[%i].type:%i\n",i,currentEntry[i].type);
        if(currentEntry[i].type==0){
            printf("current entry reached\n");
            // set current Entry [i] as our new entry
            currentEntry[i].blockLocation = blockNo;
            currentEntry[i].blockCount = blocksNeeded;
            currentEntry[i].type = 2;

            // set name to the last item in the parsed pathh arr
            printf("parsedPath[]: %s\n",parsedPath[pathLength-1]);
            strcpy(currentEntry[i].name,parsedPath[pathLength-1]);
            //currentEntry[i].name[0] = parsedPath[pathLength-1];
            printf("Current entry name: %s\n",currentEntry[i].name);
            // write current entry to memory with new values
            printf("currenteEntry[0] block count: %i\n",currentEntry[0].blockCount);
            printf("currenteEntry[0] block loc: %i\n",currentEntry[0].blockLocation);
            LBAwrite(currentEntry,currentEntry[0].blockCount,currentEntry[0].blockLocation);
            // break from loop
            break;
        }
    }

    // the last value of the path is the name of the entry
    //strcpy(entry_p->name,parsedPath[pathLength]);

    // write new entry to memory
    LBAwrite(entry_p, blocksNeeded, blockNo);

   
    // update bitmap to signify used space
    for (int i = blockNo; i < blockNo + blocksNeeded; i++) {
        freeMap[i] = 1;
    }

    // free malloc'd entry now that it is written in memory
    free(currentEntry);
    free(entry_p);

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

    // parse file path to global variable parsedPath
    parsePath(pathname);

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
        return -1;
    }

    // check if directory has children or not
    if(currentEntry[0].childrenAmount != 0){
        printf("CANNOT DELETE DIRECTORY WITH ENTRIES");
        return -1;
    }

    // variables needed to find parent entry
    entryStruct *parentEntry;
    int entrySize = sizeof(entryStruct) * DIRENTRIES;
    parentEntry = malloc(entrySize);
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
        }
    }

    // print if deleted
    printf("DIRECTORY DELETED\n");

    // mark bitmap space to 0 to signify space is free
    for (int i = blockNo; i < totalBlocks + 1; i++) {
        freeMap[i] = 0;
    }

    // free malloc'd space
    free(parentEntry);
    free(currentEntry);

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
    printf("GET CWD FUNCTION\n");
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
    // parse requested file path
    //char *parsedPath = parsePath(buf);
    parsePath(buf);

    // Check validity of path
    if(fs_isDir(buf) != 1){
        printf("%s is not a valid directory.\n", buf);
        return -1;
    }

    // search for directory based on path
    //entryStruct *entry_p;
    int pathLength = getArrLength(parsedPath);
    getEntryFromPath(parsedPath, pathLength);

    // clear old path
    currentDirectoryPath[0] = '\0';
    // ******* INVALID ********
    //currentDirectoryPathArraySize = 0;

    // set new CWD by copying parsed path to CurrDirPathArr
    for(int i = 0; i < getArrLength(parsedPath); i++) {
        strcpy(currentDirectoryPath[i], parsedPath[i]);
        // fix this
        //sprintf(currentDirectoryPath, "%s,%s", currentDirectoryPath, buf[i]);
        // ********* INVALID *********
        //currentDirectoryPathArraySize++;
    }

    printf("Setting cwd to %s.\n", currentDirectoryPath);

    return 0;
}