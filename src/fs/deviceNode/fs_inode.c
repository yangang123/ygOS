
#include "fs.h"

inode_t inode_table[INODE_NUM_MAX];
struct list_head  ygos_inode_free_list;
inode_t *ygos_inode_list;
struct inode *g_root_inode = NULL;

//插入节点: node_parent: 父节点， left_sibling：是否存在左兄弟节点  
static int  ygos_tree_insert_node(inode_t *node_parent, inode_t *node,  inode_t *left_sibling)
{   
    if (node == NULL){
        return -1;
    }

    if (left_sibling) {
        /*
         插入到右节点,由于是1个首字母排序设备树， 如下图所示，存在新节点的首字母大于console的首字母
            dev
            /
         console
              \
              led0
         -----------------
         dev
            /
         console
            \
            @new
              \
              led0
        */
      node->r_sibling = left_sibling->r_sibling;
      left_sibling->r_sibling = node; 
        
    } else if (node_parent){
         /*
          插入当前目录的第一个节点
           dev
            /
         @new
            \
             null
         -----------------
            dev
            /
         @new
            \
             null
        */
        node->r_sibling = node_parent->l_child;
        node_parent->l_child = node;

    } else {
         /*
           插入根目录
           etc  <-------------root 
            /
           rc
            \
             rcS
         -----------------
             dev <----------------root
                \
                etc
                 /
                rc
                  \
                  rcS
        */
        node->r_sibling = g_root_inode;     
        g_root_inode = node; 
    }
	
	return 0;
}

//给节点拷贝名字，不拷贝'/'
static void ygos_inode_namecpy(char *dest, const char *src)
{  
	while (*src && *src != '/') {
		*dest++ = *src++;  
	}

	*dest = '\0';
}

//分配节点
static struct inode *ygos_inode_alloc( const char *name)
{
	struct list_head *first = list_pop(&ygos_inode_free_list);
    struct inode *node = list_entry(first, inode_t, list);
    
    node->l_child =NULL;
    node->r_sibling = NULL;
	if (node)
    {  
        ygos_inode_namecpy(node->i_name, name);
    }

	return node;
}

// 删除节点内存空间
int ygos_inode_free(struct inode *node)
{  
	struct list_head * current = &node->list; 
    list_add_tail(current, &ygos_inode_free_list);
	return 0;
}

//发现inode的下一个名字/dev/ttyS0, name[0]: dev name[1]
//第一步骤： 去掉 ‘/’
//第二步骤： 传入path[1], 直到遇到'/' 或者是'\0'表示结束，返回dev,然后
//返回：下一个不带‘/'的路径
const char *ygos_inode_nextname( const char *name)
{   
	//非'/'路劲符号,就遍历下一个
    while (*name && *name != '/')
    {
      name++;
    }

	//当遇到'/'
	if (*name == '/'){
		name++;
	}

	return name;
}

// name: 当前路径的名字， nodename： 节点的名字
// 1: 表示name >节点的名字
//-1: 表示name < 节点的名字
// 0: 相等
// 注意： name遇到\0或者是/判断结束， nodename是不可能有‘/’
int ygos_inode_compare( const char *name, const char *nodename)
{    
    if (!*nodename)
    {   
        DEBUG_LR("*nodename is 0");
        return 1;
    }  
    
    if (!*name) {
        DEBUG_LR("*name");
        return -1;
    }

    for(;;){   
           
        if (!*nodename) {
           //第1个节点的名字提前结束了
           if (!*name || *name == '/') {
               return 0;
           } else {
               //路径的名字还没有结束
               return 1;
           }
           
        } else if (!*name || *name == '/') {
            //路径已经结束或者遇到遇到'/'
            return -1;
        } 
        if (*name > *nodename) {
             return 1;
        } else if (*name < *nodename) {
             return -1;
        } else {
            name++;
            nodename++;
        }
    }
}

//通过path获取设备节点
int  ygos_inode_find(const char*path, inode_desc_t *desc) 
{   
    const char	*name   = path;
	const char	*relative_path   = NULL;
    inode_t		*node   = g_root_inode;
    inode_t		*parent = NULL;
    inode_t		*left   = NULL;
    
    int ret         = -1;
    //跳过首字母"///"
    while (*name == '/')
    {
        name++;
    }

    //设备节点是按照首字母排序的
    while(node) {

        int result = 0;
        
        result = ygos_inode_compare(name, node->i_name);
        DEBUG("ygos_inode_compare: ret: %d, name:%s, node->i_name:%s\n", ret, name, node->i_name);
        //插入当前节点的左边
        if (result < 0) {
            //第一次可能直接找到
            node = NULL;
            break;

        } else if (result > 0) {
            //继续查找
            left = node;
            node = node->r_sibling;
        } else {
            //继续查找子目录
            const char *nextname = ygos_inode_nextname(name);
            if (!*nextname || FILE_NODE_TYPE_IS_MOUNTPT(node)) {
                //当前节点已经存在
                relative_path = nextname;
                ret = 1;
                break;
            }
            
            //node更新为当前节点的孩子节点
            name   = nextname;
            parent = node;
            left   = NULL;
            node   = node->l_child; 
        } 
     } 

     desc->left    = left;
     desc->parent  = parent;
     desc->path    = name;
     desc->node    = node;
     desc->relative_path    = relative_path;
     DEBUG("desc->left:%x\tdesc->parent:%x\tdesc->node:%x, relative_path=%s\n",
        desc->left,desc->parent,desc->node, desc->relative_path);
     return ret;
}

//设备树遍历
int ygos_inode_foreach(inode_t *inode, struct inode_path_s *info)
{  
    if (!inode) {
        return -1;
    }

    for (; inode; inode=inode->r_sibling) {
    
       //获取上层目录的路径
       int len = strlen(info->path);
       sprintf(&info->path[len], "/%s", inode->i_name);

       info->handler(inode, info);
       ygos_inode_foreach(inode->l_child, info);
    
       //清除
       info->path[len] = '\0';
    }
	
	return 0;
}

//获取设备node
int  ygos_inode_get(const char*path, inode_t **inode) 
{   
	const char *name = path;
    struct inode *node = NULL; 
	struct inode *parent = NULL; 
    struct inode *left = NULL;
    
    *inode = NULL;

	//路径首字母无效
	if (path[0] == '\0' || path[0] != '/')
	{   
        DEBUG("path invalid\n");
		return -1;
	}

    inode_desc_t desc;
    desc.left = NULL;
    desc.parent = NULL;
    desc.path = NULL;
    
    DEBUG("ygos_inode_get path: %s\n", path);
    
    int ret = ygos_inode_find(path, &desc);
    if(ret >= 0) {
        DEBUG("path exsit\n");
        //节点存在
        return -1;
    }
   
    //更新节点的位置
    left =  desc.left;
    parent = desc.parent;
    name = desc.path;
    DEBUG("inode_get2 name: %s\n", name);
	//比较根节点的字符
	for (; ; )
    {
       //dev/ttyS0, 这里应该得到是ttyS0
	   const char *next_name = ygos_inode_nextname(name);
       DEBUG("path: %s, next_name:%s\n", path, next_name);
       if (*next_name) {
		   //找到一级目
          node = ygos_inode_alloc(name);
		  if (node) {
			  ygos_tree_insert_node(parent, node, left);
			  parent = node;
              name   = next_name; //更新name
              DEBUG("insert parent:%s\n", (char*)node->i_name);
			  continue;
		  }

	   } else {
          //路径已经结束 dev或者是dev/ name = &path[1];
		  node = ygos_inode_alloc(name);
		  if (node) {
			  ygos_tree_insert_node(parent, node, left); 
             *inode = node;
              DEBUG("insert node:%s\n", (char*)node->i_name);
			  return 1;
		  }
	   }
       
       break;
	}
	
    return 0;
}

// 删除节点
inode_t * ygos_tree_unlink(const char* path)
{   
    inode_desc_t desc;
    desc.left = NULL;
    desc.parent = NULL;
    desc.path = NULL;
    
    DEBUG("b\n");

    int ret = ygos_inode_find(path, &desc);
    if (ret < 0) {
        DEBUG("no found\n");
        return NULL;
    }
    
    inode_t *node_parent    = desc.parent;
    inode_t *node           = desc.node;
    inode_t *left_sibling   = desc.left;

    if (node == NULL){
        DEBUG("null node\n");
        return NULL;
    }
     
    DEBUG("c\n");
    if (left_sibling) {

      left_sibling->r_sibling = node->r_sibling;   
    } else if (node_parent){

        node_parent->l_child = node->r_sibling;

    } else {
        //当前节点的兄弟节点作为根节点
        g_root_inode = node->r_sibling; 
    }

    node->r_sibling = NULL;
    DEBUG("d\n");
    return node;
}

//初始化inode的空闲链表空间
void ygos_inode_list_init(void)
{   
	//初始化空闲链表头节点
	INIT_LIST_HEAD(&ygos_inode_free_list);
	
	//把所有内存节点链表节点写入添加到空闲链表中
	for (int i =0; i < INODE_NUM_MAX; i++) {
		list_add_tail(&inode_table[i].list, &ygos_inode_free_list); 
	}
}

