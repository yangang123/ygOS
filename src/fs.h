#ifndef _FS_H
#define _FS_H

#define O_RDONLY  0x01
#define O_WRONLY  0x02

#include "list.h"

typedef struct inode inode_t;

struct file_operations
{
  int     (*open)( struct file *filep);

  int     (*close)( struct file *filep);
  int (*read)( struct file *filep,  char *buffer, int buflen);
  int (*write)( struct file *filep,  const char *buffer, int buflen);
  int     (*ioctl)( struct file *filep, int cmd, unsigned long arg);
};

//头部
#  define current_task(cpu)      ((FAR struct tcb_s *)g_readytorun.head)
#  define this_cpu()             (0)
#  define this_task()            (current_task(this_cpu()))

union inode_ops_u
{
  FAR const struct file_operations     *i_ops;    /* Driver operations for inode */
};

struct inode {
    struct node *l_child;
    struct node *r_child;
    void  *i_private;  
	struct list_head list;
    union inode_ops_u u; 
    char  i_name[1];         
};

#define INODE_NUM_MAX  20

//定义文件句柄的最大数量
#define CONFIG_NFILE_DESCRIPTORS           8

struct file
{
  int               f_oflags;   
//   off_t             f_pos;      /* File position */
  struct inode *f_inode;  
  void             *f_priv;    
};

//
struct filelist
{
  //sem_t   fl_sem;               /* Manage access to the file list */
  struct file fl_files[CONFIG_NFILE_DESCRIPTORS];
};

#endif 