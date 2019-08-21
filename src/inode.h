#ifndef INODE_H
#define INODE_H

#include "list.h"

typedef struct node inode_t;
	
struct node {
    struct node *l_child;
    struct node *r_child;
    void  *i_private;  /* Per inode driver private data */
    char  i_name[1];  /* Name of inode (variable) */
	struct list_head list;
};

#define INODE_NUM_MAX  20

	
#endif
