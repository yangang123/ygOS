
#include "fs.h"

inode_t inode_table[INODE_NUM_MAX];
struct list_head  ygos_inode_free_list;
inode_t *ygos_inode_list;
struct inode *g_root_inode = NULL;

//初始化TCB的空闲链表空间
void ygos_inode_list_init(void)
{   
	//初始化空闲链表头节点
	INIT_LIST_HEAD(&ygos_inode_free_list);
	
	//把所有内存节点链表节点写入添加到空闲链表中
	for (int i =0; i < INODE_NUM_MAX; i++) {
		list_add_tail(&inode_table[i].list, &ygos_inode_free_list); 
	}
}


