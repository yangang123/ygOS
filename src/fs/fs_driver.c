
#include "fs.h"
#include "list.h"

void inode_lock(void)
{

}


void inode_unlock(void)
{

}


int  inode_get(const char*path, inode_t **node) 
{   
	struct list_head *first = list_get_first(&ygos_inode_free_list);
	inode_t    *pnode  = list_entry(first, inode_t, list);
	*node = pnode;
	
	memcpy(pnode->i_name, path, strlen(path));
	
    return 0;
}

int register_driver(const char*path, const struct file_operations *fops, void *priv)
{  
    inode_t *node;
    int ret = -1;

    inode_lock();

    ret = inode_get(path, &node);
    if (ret < 0) {
        inode_unlock();
        return -1;
    }
    node->i_private = priv;
    
    inode_unlock();
    return 0;
}


