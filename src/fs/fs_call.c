#include "fs.h"
#include "rtos.h"


//文件打开
int file_open( struct file *filep,  void *buf, int nbytes)
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

//关闭
int file_close(struct file *filep)
{
   struct inode *inode;
   inode = filep->f_inode;

   return inode->u.i_ops->close(filep);
}

//打开
int open( const char * pathname, int flags)
{
    int fd = -1;
    return fd;
}


//关闭
int read(int fd,void * buf ,int count)
{
    if (fd < 0 ) {
		return -1;
	}
    
    //得到当前任务
    struct file *filep = &ygos_tcb_current->tg_filelist.fl_files[fd];
    return file_read(filep, buf, count);
}

//写入
int write(int fd,void * buf ,int count)
{
    if (fd < 0 ) {
		return -1;
	}
    
	struct file *filep = &ygos_tcb_current->tg_filelist.fl_files[fd];
    return file_write(filep, buf, count);
}

int close(fd)
{
    return 0;
}