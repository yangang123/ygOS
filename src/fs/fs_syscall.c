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
   return inode->u.i_ops->read(filep, buf, nbytes);
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

//通过fd,获取file对象
struct file* file_get(int fd)
{   
    return &ygos_tcb_current->tg_filelist.fl_files[fd];
}

//打开
int ygopen( const char * path, int oflags, ...)
{    
    int fd = -1;
    int ret = -1;

    //查找路径
    inode_desc_t desc;
    INODE_INIT(desc,path);
    ret= ygos_inode_find(path, &desc);
    if (ret < 0) {
        DEBUG("no found\n");
        return -1;
    }
    DEBUG("desc->left:%x\tdesc->parent:%x\tdesc->node:%x\n",desc.left,desc.parent,desc.node);
    //当前设备节点添加到到文件
    inode_t *node = desc.node;
    fd = file_alloc(node);
    DEBUG("fd:%d\n", fd);
    struct file *filep = file_get(fd);
    if (fd >= 0) {  
        if (FILE_NODE_TYPE_IS_MOUNTPT(node))    {
            ret= node->u.i_mops->open(filep, desc.relative_path, oflags, 0);
           
        }   else    {
            ret= node->u.i_ops->open(filep);
        }
        if (ret < 0 ) {
             DEBUG_LR("open failed:%d", ret);
            return -1;
        }
    }
    return fd;
}


//关闭
int ygread(int fd,void * buf ,int count)
{  
    if (fd < 0 ) {
		return -1;
	}
    //得到当前任务
    struct file *filep = file_get(fd);
    return file_read(filep, buf, count);
}

//写入
int ygwrite(int fd,void * buf ,int count)
{
    if (fd < 0 ) {
		return -1;
	}
    
	struct file *filep = file_get(fd);
    return file_write(filep, buf, count);
}

//关闭
int ygclose(int fd)
{
    return 0;
}
