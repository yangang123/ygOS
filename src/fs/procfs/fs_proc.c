
#include "procfs.h"

int     procfs_open( struct file *filep,  const char *relpath,
        int oflags, mode_t mode);
int     procfs_close( struct file *filep);
int     procfs_read( struct file *filep,  char *buffer, int buflen);
int     procfs_bind( struct inode *blkdriver,  const void *data,
            void **handle);
int     procfs_unbind( void *handle,  struct inode **blkdriver,
        unsigned int flags);

extern struct procfs_operations  uptime_operations;
static const struct procfs_entry_s g_base_entries[] =
{
        { "uptime",        &uptime_operations,          PROCFS_FILE_TYPE   }, 
};
const int g_base_entries_count =
    sizeof(g_base_entries)/sizeof(struct procfs_entry_s);
const struct mountpt_operations procfs_operations =
{
    .open   =procfs_open,
    .close  =procfs_close,
    .read   =procfs_read,
    .write  =NULL,
    .seek   =NULL,
    .ioctl  =NULL,

    .sync   =NULL,
    .dup    =NULL,

    .bind   =procfs_bind,
    .unbind =procfs_unbind
};

//打开设备节点
int procfs_open( struct file *filep,  const char *relpath,
        int oflags, mode_t mode)
{   
    int ret= -1;

    for(int i = 0; i < g_base_entries_count; i++) {
        DEBUG_LR("pathpattern:%s", g_base_entries[i].pathpattern);
        if (strcmp(relpath, g_base_entries[i].pathpattern) == 0) {
            DEBUG_LR("find path:%s", relpath);    
            ret = g_base_entries[i].ops->open(filep, relpath, oflags, mode);    
            if(ret == 0) {
                //filep->f_priv在g_base_entries[i].ops->open中分配了内存空间，
                //使用了面向对象的思想
                struct procfs_file_s *f_priv_object = 
                        (struct procfs_file_s *)filep->f_priv;
                f_priv_object->procfsentry = &g_base_entries[i];
                break;
            }
        }
    }
    return ret;
}

int procfs_close( struct file *filep)
{ 
	return 0;
} 

//读取proc文件内容
int procfs_read( struct file *filep,  char *buffer, int buflen)
{   
    struct procfs_file_s   *handler = (struct procfs_file_s *)(filep->f_priv);
    
    //获取proc子目录的文件句柄，然后获取数据
    return handler->procfsentry->ops->read(filep, buffer, buflen);
} 

int procfs_bind( struct inode *blkdriver,  const void *data, void **handle)
{   
        
    return 0;
} 

int procfs_unbind( void *handle,  struct inode **blkdriver, unsigned int flags)
{ 
	return 0;
} 
