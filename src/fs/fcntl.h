#ifndef _FCNTL_H
#define _FCNTL_H

int open( const char * pathname, int flags);
int read(int fd,void * buf ,int count);
int write(int fd,void * buf ,int count);

#endif 