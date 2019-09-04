#ifndef _PROCFS_H
#define _PROCFS_H

#include <ygos/fs/fs.h>
#include <ygos/libc/stdint.h>

// proc对子目录的抽象
struct procfs_operations
{
    int (*open)( struct file *filep,  const char *relpath,
                int oflags, mode_t mode);
    int (*close)( struct file *filep);
    int (*read)( struct file *filep,  char *buffer, size_t buflen);
    int (*write)( struct file *filep,  const char *buffer, size_t buflen);
};

//proc子目录描述
struct procfs_entry_s
{
    const char *pathpattern;
    const struct procfs_operations *ops;
    unsigned char type;
};

struct procfs_file_s
{
  const struct procfs_entry_s *procfsentry;
};

// proc 对uptime的定义
struct uptime_file_s
{
    struct procfs_file_s  base;        /* Base open file structure */
    char line[20];  
};

#endif /*_PROCFS_H*/
