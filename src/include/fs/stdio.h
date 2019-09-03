#ifndef _STDIO_H
#define _STDIO_H

#include "list.h"

#include <string.h>
#include <stdio.h>

#ifndef LINUX
	#define ygopen 	open
	#define ygread 	read
	#define ygwrite write
#endif

int ygopen( const char * path, int oflags, ...);
int ygread(int fd,void * buf ,int count);
int ygwrite(int fd,void * buf ,int count);
int ygclose(int fd);

#endif 
