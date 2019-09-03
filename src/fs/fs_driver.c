
#include "fs.h"
#include "list.h"

void inode_lock(void)
{

}

void inode_unlock(void)
{

}

// 注册设备驱动程序
int register_driver(const char*path, const struct file_operations *fops, void *priv)
{  
    inode_t *node;
    int ret = -1;
    ret = ygos_inode_get(path, &node);
    if (ret < 0) {
        return -1;
    }
    node->u.i_ops   = fops;
    node->i_private = priv;
    FILE_NODE_TYPE_IS_CHAR(node);
    
    return 0;
}

//移除字符设备驱动
int unregister_driver(const char*path)
{    
    DEBUG("a\n");
    inode_t *inode;
    inode = ygos_tree_unlink(path);
    ygos_inode_free(inode);
    return 0;
}


