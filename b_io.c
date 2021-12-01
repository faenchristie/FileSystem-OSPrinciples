/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: b_io.c
*
* Description: Basic File System - Key File I/O Operations
* 
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // for malloc
#include <string.h> // for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fsLow.h"
#include "b_io.h"
#include "fsInit.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
{
	entryStruct *fi; //holds the low level systems file info
	// Add any other needed variables here to track the individual open file
	// file id / location in array
	int id;
	char *f_buffer; // our buffer for reading file
	int buflen;
	int index;
	int f_offset; // offset of file 
	int b_offset; // offset of buffer(when partial)
	// block location, track amount already read to user
	int loc;
	int read;
	int sizeOfFile;
} b_fcb;

b_fcb fcbArray[MAXFCBS];
uint64_t bufSize;

int startup = 0; //Indicates that this has not been initialized

//Method to initialize our file system
void b_init() {

	bufSize = vcb_p->blockSize;

	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
	{
		fcbArray[i].f_buffer = NULL; //indicates a free fcbArray
	}

	startup = 1;
}

//Method to get a free FCB element
b_io_fd b_getFCB()
{
	for (int i = 0; i < MAXFCBS; i++)
	{
		if (fcbArray[i].f_buffer == NULL)
		{
			return i; //Not thread safe (But do not worry about it for this assignment)
		}
	}
	return (-1); //all in use
}

/******************************************************************************
 * 				-----Interface to open a buffered file-----
 * 
 * Modification of interface for this assignment, flags match the linux flags
 * for open
 * O_RDONLY, O_WRONLY, or O_RDWR
 * First, initialize if need be
 * then an error check for the return fcb to make sure we actually got it
 * malloc to the size of our vcb
 * another error check if there isnt enough space
 * set the buffer length and index to 0
 * 
 * @return returnFD
 *****************************************************************************/
b_io_fd b_open(char *filename, int flags) {

	b_io_fd fd; 
	b_io_fd returnFd;
	b_fcb * fcb;

	if (startup == 0)
		b_init(); //Initialize our system

	returnFd = b_getFCB(); // get our own file descriptor
	// check for error - all used FCB's

	//error check for return fcb
	if(returnFd < 0) {
		return -1;
	}

	fcb = &fcbArray[returnFd];

	//allocate the buffer to the size of our vcb
	fcb->f_buffer = malloc(bufSize + 1);

	//error check for if there is not enough space
	if (fcb->f_buffer == NULL) {
		close(fd);
		return -1;
	}

	fcb->buflen = 0;
	fcb->index = 0;

	//printf("b_open: opening file '%s' with file descriptor %d\n", filename, fd);

	return (returnFd); 
}

/******************************************************************************
 * 					-----Interface to seek function-----
 * 
 * repositions the file offset of the open file description 
 * 
 * Initialize system if need be and do an error check
 * next, if else if statements for SEEK_SET, SEEK_CUR, and SEEK_END.
 * We need to reposition the file pointer and we do that with the above
 * the first, SEEK_SET will be placed to the location of the data + the offset
 * the next, SEEK_CUR will be placed at the location += offset
 * the last SEEK_END will be placed at the end of the file, so we use fileSize
 * here + the offset.
 * Also created an else statement at the end that will just return -1 error 
 * if the value of the origin is invalid.
 * 
 * @return fcbArray[fd].loc
 *****************************************************************************/
int b_seek(b_io_fd fd, off_t offset, int whence) {
	if (startup == 0)
		b_init(); //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS)) {
		return (-1); //invalid file descriptor
	}

	if(whence = SEEK_SET) {
		fcbArray[fd].loc = offset;
		return fcbArray[fd].loc;
	}

	else if(whence = SEEK_CUR) {
		fcbArray[fd].loc = fcbArray[fd].loc + offset;
		return fcbArray[fd].loc;

	}

	else if(whence = SEEK_END){
		fcbArray[fd].loc = fcbArray[fd].sizeOfFile + offset;
		return fcbArray[fd].loc;

	} 
	// print error if not valid function parameter
	else{
		printf("invalid value of whence for this function.\n");
		return -1;
	}
}

/******************************************************************************
 * 					-----Interface to write function-----
 * 
 * creating a few variables to get the bytes we need copied
 * initialize system if need be, and error check for the file descriptor
 * 
 * first, created an if else statement to determine what the byteCopied will
 * consist of originally, either the count, or the freeSpace, based on the 
 * values of either of them.
 * call a memcpy of the buffer to the buffer + index with the amount of bytes
 * that were copied.
 * the next block of code is incomplete but the goal is to find the next free
 * block available and then write the data to disk, throwing an error message
 * if there isnt enough space. then do another memcpy of the next amount of 
 * bytes copied which was our other variable for the leftover bytes.
 * 
 * ISSUE: needs comment portion completed
 * 
 * @return totalBytesCopied
 *****************************************************************************/
int b_write(b_io_fd fd, char *buffer, int count) {
	
	int bytesCopied;
	b_fcb * fcb;
	int nextBytesCopied = count - bytesCopied;
	int freeSpace = bufSize - fcb->index;

	if (startup == 0)
		b_init(); //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS)) {
		return (-1); //invalid file descriptor
	}

	if (freeSpace > count) {
		bytesCopied = count;
	} else {
		bytesCopied = freeSpace;
	}

	//copy buffer to the buffer + index
	memcpy(fcb->f_buffer + fcb->index, buffer, bytesCopied);
	fcb->index = fcb->index + bytesCopied;

	if (nextBytesCopied != 0) {
		//find the next free block
		//if (block is not free)
			//throw error if there is not enough space to write
		//else (write the data to disk)
	fcb->index = 0;
	//copy buffer + bytecopied to buffer + index
	memcpy(fcb->f_buffer + fcb->index, buffer + bytesCopied, nextBytesCopied);
	fcb->index = fcb->index + nextBytesCopied;
	}
	
	int totalBytesCopied = bytesCopied + nextBytesCopied;
	return totalBytesCopied;
}

/******************************************************************************
 * 					-----Interface to read a buffer-----
 * 
 * Filling the callers request is broken into three parts
 * Part 1 is what can be filled from the current buffer, which may or may not 
 * be enough
 * 
 * Part 2 is after using what was left in our buffer there is still 1 or more
 * block size chunks needed to fill the callers request.  This represents the 
 * number of bytes in multiples of the blocksize.
 * 
 * Part 3 is a value less than blocksize which is what remains to copy to the 
 * callers buffer after fulfilling part 1 and part 2.  This would always be 
 * filled from a refill of our buffer.
 * 
 * Used mostly team member Natalie Christie's logic from Assignment 2b 
 * 
 * +-------------+------------------------------------------------+--------+
 * |             |                                                |        |
 * | filled from |  filled direct in multiples of the block size  | filled |
 * | existing    |                                                | from   |
 * | buffer      |                                                |refilled|
 * |             |                                                | buffer |
 * |             |                                                |        |
 * | Part1       |  Part 2                                        | Part3  |
 * +-------------+------------------------------------------------+--------+
 *****************************************************************************/    
int b_read(b_io_fd fd, char *buffer, int count) {

	if (startup == 0)
		b_init(); //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
	{
		return (-1); //invalid file descriptor
	}

	if (fcbArray[fd].fi == NULL) //File not open for this descriptor
	{
		return -1;
	}

	// return if amount read is greater than or equal to file size
	// return 0 to signify read is done 
	if (fcbArray[fd].read >= fcbArray[fd].fi->size)
	{
		return 0;
	}

	// keeps track of how much we have left remaining to read
	int remainder = count;

	// while loop that will keep looping until all is read
	// to the user.
	while (remainder != 0)
	{

		// initialize buffer if empty, do first LBAread, increment loc
		// for next read
		if (strlen(buffer) == 0)
		{
			LBAread(fcbArray[fd].f_buffer, 1, fcbArray[fd].loc);
			fcbArray[fd].loc++;
		}

		// finds offset of user buffer so we don't overwrite
		// any memory 
		int user_offset = count - remainder;

		// checks if count is still greater than 512, will
		// continue looping
		if (remainder > B_CHUNK_SIZE)
		{

			// remaining amount in buffer we can copy
			int buff_amt = B_CHUNK_SIZE - fcbArray[fd].b_offset;

			// copies at offset of user buffer from offset of buffer,
			// only the amount left in buffer.
			memcpy(buffer + user_offset, fcbArray[fd].f_buffer
			 + fcbArray[fd].b_offset, buff_amt);

			// updates remaining amount that we have to read
			remainder = remainder - buff_amt;

			// copies new 512 block into buffer
			LBAread(fcbArray[fd].f_buffer, 1, fcbArray[fd].loc);

			// increment location of block
			fcbArray[fd].loc++;

			// update amount read to user
			fcbArray[fd].read += count;	

			// updates buffer offset to 0 as we have copied final amount
			fcbArray[fd].b_offset = 0;
		}
		else
		{
			// if remaining amount is less than the amount left in buffer,
			// no need to LBAread
			if (remainder < B_CHUNK_SIZE - fcbArray[fd].b_offset)
			{
				// copies remaining amount to user buffer at offset
				memcpy(buffer + user_offset, fcbArray[fd].f_buffer + fcbArray[fd].b_offset,
					   remainder);

				// buffer offset is whatever amount we have just read
				fcbArray[fd].b_offset += remainder;
				fcbArray[fd].read += count;

				// nothing left to read
				remainder = 0;
			}

			else
			{

				// remaining amount in buffer
				int rem_buff = B_CHUNK_SIZE - fcbArray[fd].b_offset;

				// calculate remaining amount after buffer
				int after_buff = remainder - rem_buff;

				// copy remaining amount in buffer to user
				memcpy(buffer + user_offset, fcbArray[fd].f_buffer + fcbArray[fd].b_offset,
					   rem_buff);

				// increment offset of user buffer as to not overwrite
				user_offset += rem_buff;

				// read one block of 512 bytes into our buffer
				LBAread(fcbArray[fd].f_buffer, 1, fcbArray[fd].loc);

				// copy remaining amount from buffer to user
				memcpy(buffer + user_offset, fcbArray[fd].f_buffer, after_buff);

				// what was just copied is now the buffer offset
				fcbArray[fd].b_offset = after_buff;

				// increment location of file block
				fcbArray[fd].loc += 1;

				// increment amount read to user
				fcbArray[fd].read += count;

				remainder = 0;
			}
		}
	}
	// return amount read to the user
	return count;
}

// Interface to Close the file
void b_close(b_io_fd fd) {

	// free what was malloc'd
	/*
	free(fcbArray[fd]);
	fcbArray[fd] == NULL;
	*/
	
}

