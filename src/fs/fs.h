#ifndef _FS_H
#define _FS_H



#include "list.h"

#define O_RDONLY  0x01
#define O_WRONLY  0x02

#define INODE_NUM_MAX  20

//定义文件句柄的最大数量
#define CONFIG_NFILE_DESCRIPTORS           8

struct file;

struct file_operations
{
  int (*open)( struct file *filep);
  int (*close)( struct file *filep);
  int (*read)( struct file *filep,  char *buffer, int buflen);
  int (*write)( struct file *filep,  const char *buffer, int buflen);
  int (*ioctl)( struct file *filep, int cmd, unsigned long arg);
};

union inode_ops_u
{
  const struct file_operations     *i_ops;    
};

struct inode {
	struct inode *l_child;
	struct inode *r_child;
	void  *i_private;  
	struct list_head list;
	union inode_ops_u u; 
	char  i_name[20];         
};
typedef struct inode inode_t;

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

extern struct list_head  ygos_inode_free_list;

int register_driver(const char*path, const struct file_operations *fops, void *priv);

#endif 
