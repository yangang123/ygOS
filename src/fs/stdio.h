#ifndef _STDIO_H
#define _STDIO_H

#include "list.h"

#include <string.h>
#include <stdio.h>

int open( const char * path, int flags);
int read(int fd,void * buf ,int count);
int write(int fd,void * buf ,int count);
int close(int fd);

#endif 
