#ifndef _FS_H
#define _FS_H

#include "list.h"

#include <string.h>
#include <stdio.h>

#ifdef DEBUG
#define DEBUG(...)	printf(__VA_ARGS__)
#define DEBUG_LR(...)	printf(__VA_ARGS__);printf("\n")
#else 
#define DEBUG(...)	
#define DEBUG_LR(...)	
#endif 

#define O_RDONLY                            0x01
#define O_WRONLY                            0x02

#define INODE_NUM_MAX                       8

//定义文件句柄的最大数量
#define CONFIG_NFILE_DESCRIPTORS            8

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
    struct inode *r_sibling;
    void  *i_private;  
    struct list_head list;
    union inode_ops_u u; 
    char  i_name[20];         
};
typedef struct inode inode_t;

struct file
{
    int               f_oflags;   
    struct inode     *f_inode;  
    void             *f_priv;    
};

struct filelist
{          
    struct file fl_files[CONFIG_NFILE_DESCRIPTORS];
};

typedef struct desc {
    inode_t *parent;
    inode_t *left;
    const char *path;
    inode_t *node;
}inode_desc_t;

#define INODE_INIT(desc,path)\
  desc.path=path;\
  desc.parent=NULL;\
  desc.left=NULL;\
  desc.left=NULL

struct inode_path_s;
typedef void (*foreach_inode_t)(inode_t*, struct inode_path_s *);
struct inode_path_s
{
    foreach_inode_t handler;
    char            path[40];
};

extern struct list_head  ygos_inode_free_list;
extern struct inode *g_root_inode;


int 	inode_foreach(inode_t *inode, struct inode_path_s *info);
inode_t * 	ygos_tree_unlink(const char* path);
int 	ygos_inode_free(struct inode *node);
void 	ygos_inode_list_init(void);
int  	inode_find(const char*path, inode_desc_t *desc);

int 	register_driver(const char*path, const struct file_operations *fops, void *priv);
int 	unregister_driver(const char*path);
int 	inode_get(const char*path, inode_t **inode);

#endif 
