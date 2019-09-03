#ifndef _FS_H
#define _FS_H

#include "list.h"
#include "stdio.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef DEBUG_ON
#define DEBUG(...)	    printf("function:%s\tline:%03d\t",__FUNCTION__,__LINE__); printf(__VA_ARGS__)
#define DEBUG_LR(...)	printf("function:%s\tline:%03d\t",__FUNCTION__,__LINE__); printf(__VA_ARGS__);printf("\n")
#define DEBUG_COMMON(...) printf(__VA_ARGS__)
#else 
#define DEBUG(...)	
#define DEBUG_LR(...)	
#define DEBUG_COMMON(...) 
#endif 

#define O_RDONLY                            0x01
#define O_WRONLY                            0x02

#define INODE_NUM_MAX                       8
#define FAT_FILE_NUM                        32
//定义文件句柄的最大数量
#define CONFIG_NFILE_DESCRIPTORS            8

#define FILE_NODE_TYPE_MASK                 0x00000007
#define FILE_NODE_TYPE_CHAR                 0x00000000 /*   Character driver         */
#define FILE_NODE_TYPE_BLOCK                0x00000001 /*   Block driver             */
#define FILE_NODE_TYPE_MOUNTPT              0x00000002 /*   Mount point              */

#define FILE_NODE_IS_TYPE(i,t)              (((i)->i_flags & FILE_NODE_TYPE_MASK) == (t)) 
#define FILE_NODE_GET_TYPE(i)               ((i)->i_flags & FILE_NODE_TYPE_MASK)  
#define FILE_NODE_SET_TYPE(i,t)             i->i_flags = ((i)->i_flags & ~FILE_NODE_TYPE_MASK) | (t)

#define FILE_NODE_TYPE_SET_CHAR(i)          FILE_NODE_SET_TYPE(i, FILE_NODE_TYPE_CHAR)
#define FILE_NODE_TYPE_SET_BLOCK(i)         FILE_NODE_SET_TYPE(i, FILE_NODE_TYPE_BLOCK)
#define FILE_NODE_TYPE_SET_MOUNTPT(i)       FILE_NODE_SET_TYPE(i, FILE_NODE_TYPE_MOUNTPT)

#define FILE_NODE_TYPE_IS_CHAR(i)           FILE_NODE_IS_TYPE(i, FILE_NODE_TYPE_CHAR)
#define FILE_NODE_TYPE_IS_BLOCK(i)          FILE_NODE_IS_TYPE(i, FILE_NODE_TYPE_BLOCK)
#define FILE_NODE_TYPE_IS_MOUNTPT(i)        FILE_NODE_IS_TYPE(i, FILE_NODE_TYPE_MOUNTPT)

typedef unsigned char mode_t;
typedef unsigned int off_t;
 

#define INODE_INIT(desc,path)\
  desc.path=path;\
  desc.parent=NULL;\
  desc.left=NULL;\
  desc.left=NULL

//文件操作接口
struct file;
struct file_operations
{
    int (*open)( struct file *filep);
    int (*close)( struct file *filep);
    int (*read)( struct file *filep,  char *buffer, int buflen);
    int (*write)( struct file *filep,  const char *buffer, int buflen);
    int (*ioctl)( struct file *filep, int cmd, unsigned long arg);
};

//文件屬性
enum procfs_entry_e
{
  PROCFS_UNKOWN_TYPE = 0, 
  PROCFS_FILE_TYPE,       
  PROCFS_DIR_TYPE,        
};

//文件节点属性
union inode_ops_u
{
  const struct file_operations     *i_ops;
  const struct mountpt_operations  *i_mops;
};

//定义设备节点
struct inode {
    struct inode    *l_child;
    struct inode    *r_sibling;
    void            *i_private;  
    struct          list_head list;
    int             i_flags;   
    union           inode_ops_u u; 
    char            i_name[20];         
};
typedef struct inode inode_t;

//文件定义
struct file
{
    int               f_oflags;   
    struct inode     *f_inode;  
    void             *f_priv;    
    off_t             f_pos;      
};

//每个任务控制块都是有个文件句柄list
struct filelist
{          
    struct file fl_files[CONFIG_NFILE_DESCRIPTORS];
};

//用来管理文件夹
struct fs_dirent_s {
	int a;
};

//描述设备节点在二叉树状态
typedef struct desc {
    inode_t *parent;
    inode_t *left;
    const char *path;
    const char *relative_path;
    inode_t *node;
}inode_desc_t;


//用于遍历设备二叉树
struct inode_path_s;
typedef void (*foreach_inode_t)(inode_t*, struct inode_path_s *);
struct inode_path_s
{
    foreach_inode_t handler;
    char            path[40];
};

// 文件系统挂载点
struct mountpt_operations
{
  int     (*open)( struct file *filep,  const char *relpath,
            int oflags, mode_t mode);

  int     (*close)( struct file *filep);
  int 	  (*read)( struct file *filep,  char *buffer, int buflen);
  int 	  (*write)( struct file *filep,  const char *buffer,
            size_t buflen);
  int     (*seek)( struct file *filep, int offset, int whence);
  int     (*ioctl)( struct file *filep, int cmd, unsigned long arg);

  int     (*sync)( struct file *filep);
  int     (*dup)( const struct file *oldp,  struct file *newp);
  
  int     (*opendir)( struct inode *mountpt,  const char *relpath,
             struct fs_dirent_s *dir);
  int     (*closedir)( struct inode *mountpt,
             struct fs_dirent_s *dir);
  int     (*readdir)( struct inode *mountpt,
             struct fs_dirent_s *dir);
  int     (*rewinddir)( struct inode *mountpt,
             struct fs_dirent_s *dir);
  int     (*bind)( struct inode *blkdriver,  const void *data,
             void **handle);
  int     (*unbind)( void *handle,  struct inode **blkdriver,
            unsigned int flags);
};

#ifdef LINUX
extern struct list_head  ygos_inode_free_list;
#endif 

//设备树根节点
extern struct inode *g_root_inode;

//注册字符设备驱动
int     register_driver(const char*path, const struct file_operations *fops,
    void *priv);

//移除字符设备驱动
int     unregister_driver(const char*path);

// 删除节点内存空间
int     ygos_inode_free(struct inode *node);

//初始化inode的空闲链表空间
void    ygos_inode_list_init(void);

//通过path获取设备节点
int     ygos_inode_find(const char*path, inode_desc_t *desc);

//通过path获取设备节点
int     ygos_inode_get(const char*path, inode_t **inode);

//设备树遍历
int     ygos_inode_foreach(inode_t *inode, struct inode_path_s *info);

// 删除节点
inode_t * 	ygos_tree_unlink(const char* path);

int mount( const char *source,  const char *target,
           const char *filesystemtype, unsigned long mountflags,
           const void *data);


#endif 
