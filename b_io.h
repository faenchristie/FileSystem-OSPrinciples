/**************************************************************
* Class:  CSC-415-03 + CSC-415-02 Fall 2021
* Names: Natalie Christie , Joseph Kois , Jade Simien , Eugene San Juan
* Student IDs: 918376646 , 920921089 , 920258687 , 918354065
* GitHub Name: nataliekchristie , josephkois , JadeS01 , esanjuan915
* Group Name: Arrays of Dread
* Project: Basic File System
*
* File: b_io.h
*
* Description: Interface of basic I/O functions
*
**************************************************************/

#ifndef _B_IO_H
#define _B_IO_H
#include <fcntl.h>

typedef int b_io_fd;

b_io_fd b_open(char *filename, int flags);
int b_read(b_io_fd fd, char *buffer, int count);
int b_write(b_io_fd fd, char *buffer, int count);
int b_seek(b_io_fd fd, off_t offset, int whence);
void b_close(b_io_fd fd);

#endif
