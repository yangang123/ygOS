#include "fs.h"
#include "rtos.h"


//文件打开
int file_open( struct file *filep, int flags)
{
   struct inode *inode;
   inode = filep->f_inode;
   return inode->u.i_ops->open(filep);
}

//文件读取
int file_read( struct file *filep,  void *buf, int nbytes)
{
   //执行file_node
   struct inode *inode;
   inode = filep->f_inode;
   inode->u.i_ops->read(filep, buf, nbytes);
	
   return 0;
}

//文件写入
int file_write( struct file *filep,  void *buf, int nbytes)
{
   struct inode *inode;
   inode = filep->f_inode;
   return inode->u.i_ops->write(filep, buf, nbytes);
}

//文件关闭
int file_close(struct file *filep)
{
   struct inode *inode;
   inode = filep->f_inode;
   return inode->u.i_ops->close(filep);
}

//分配文件句柄
int file_alloc(inode_t *node)
{   
    struct filelist *list= &ygos_tcb_current->tg_filelist;
    for (int i = 0; i < CONFIG_NFILE_DESCRIPTORS; i++) {
        if (!list->fl_files[i].f_inode) {
            list->fl_files[i].f_inode = node;
            list->fl_files[i].f_priv  = NULL;
            return i;
        }
    }

    return -1;
}

struct file* file_get(int fd)
{   
    return &ygos_tcb_current->tg_filelist.fl_files[fd];
}

//打开
int open( const char * path, int flags)
{    
    int fd = -1;
    
    //查找路径
    inode_desc_t desc;
    INODE_INIT(desc, path);
    int ret= inode_find(path, &desc);
   
    if (ret < 0) {
        DEBUG("no found\n");
        return -1;
    }
    DEBUG("desc->left:%x\tdesc->parent:%x\tdesc->node:%x\n",desc.left,desc.parent,desc.node);
    //当前设备节点添加到到文件
    inode_t *node = desc.node;
    fd = file_alloc(node);
    DEBUG("fd:%d\n", fd);
    if (fd >= 0) {
        return node->u.i_ops->open(file_get(fd));
    }
    return fd;
}


//关闭
int read(int fd,void * buf ,int count)
{  
    if (fd < 0 ) {
		return -1;
	}
    //得到当前任务
    struct file *filep = file_get(fd);
    return file_read(filep, buf, count);
}

//写入
int write(int fd,void * buf ,int count)
{
    if (fd < 0 ) {
		return -1;
	}
    
	struct file *filep = file_get(fd);
    return file_write(filep, buf, count);
}

//关闭
int close(int fd)
{
    return 0;
}
