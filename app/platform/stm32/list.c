#include <ygos/list.h>
#include <stdio.h>

//定义1个数据类型
typedef struct node {
    int value;
    struct list_head list;
}node_t;



//静态定义1个头节点
static LIST_HEAD(head);  //已经有内存空间,空指针

//定义3个全局变量
node_t node[3] = {
    {1, {NULL, NULL} },
    {2, {NULL, NULL} },
    {3, {NULL, NULL} }
};

void print_all_node(void)
{
     //判断节点是否是空
    if (!list_empty(&head)) {
        //遍历所有节点
        node_t *sp;
        list_for_each_entry(sp, &head, node_t, list){
            //DEBUG("sp:%x, value:%d\n", sp, sp->value);
        }
    } 
}
int main(int argc, char **argv)
{   
    //添加3个节点
    list_add_tail(&node[0].list, &head);
    list_add_tail(&node[1].list, &head);
    list_add_tail(&node[2].list, &head);

    print_all_node();

    //删除尾节点
    //DEBUG("delete tail(node[2])\n");
    list_del(head.prev);
    print_all_node();

    //DEBUG("delete tail(node[1])\n");
    list_del(head.prev);
    print_all_node();

    //DEBUG("delete tail(node[0])\n");
    list_del(head.prev);
    print_all_node();


    return 0;
}   

