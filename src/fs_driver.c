
#include "inode.h"

void inode_lock(void)
{

}


void inode_unlock(void)
{

}

int register_driver(const char*path, const struct file_operations *fops, void *priv)
{  
    inode_t *node;
    int ret = -1;

    inode_lock()

    ret = inode_get(path, &node);
    if (ret < 0) {
        inode_unlock();
        return -1;
    }

    node->i_private = priv;   

    inode_unlock();
    return 0;
}

/*
    fs_


*/


