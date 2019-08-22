#include "fs.h"
#include "rtos.h"



int file_open( struct file *filep,  void *buf, int nbytes)
{
    struct inode *inode;
   inode = filep->f_inode;
   inode->u.i_ops->open(filep);
}

int file_read( struct file *filep,  void *buf, int nbytes)
{
    //执行file_node
   struct inode *inode;
   inode = filep->f_inode;
   inode->u.i_ops->read(filep, buf, nbytes);
}

int file_write( struct file *filep,  void *buf, int nbytes)
{

}

int open( const char * pathname, int flags)
{
    int fd = -1;

  
  
    
    return fd;
}
//每个线程独立fd句柄
int read(int fd,void * buf ,int count)
{
    int fd = -1;
    
    //得到当前任务
    struct file *filep = ygos_tcb_current->tg_filelist.fl_files[fd];
    file_read(filep, buf, count);

    return fd;
}

int write(int fd,void * buf ,int count)
{
    int fd = -1;



    return fd;
}