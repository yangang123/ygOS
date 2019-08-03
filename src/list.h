#ifndef LIST_H
#define LIST_H

#ifdef LINUX
#define size_t unsigned long
#endif 

/*
struct {	 
	int a ;    //a得地址 = next成员地址-4
	int *next; //高地址
} 
*/

//把0地址强制转化成结构体地址，成员地地址就是成员在结构体中打偏移量
#define container_of(ptr, type, member)                      \
	((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member))) 


struct list_head {
	struct list_head *next, *prev;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

//初始化链表,next节点指向list,prev的节点指头	
static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}
	
//查找某个节点对应结构体打头指针
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

//查询节点
#define list_for_each_entry(pos, head, type, member)				\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))


//判断节点是否是空节点
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

//在prev节点之后，next节点之前添加一个new节点
static inline void __list_add(struct list_head *_new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

//在尾部添加节点
static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head->prev, head);
}

//删除节点
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

//内存释放后链表指向一个位置
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

//删除节点
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (struct list_head*)LIST_POISON1;
	entry->prev = (struct list_head*)LIST_POISON2;
}

//删除第一个节点
static inline void list_del_first(struct list_head *head)
{   
	if(!list_empty(head)) {
		list_del(head->next);
	}
}

//删除第二个节点
static inline void list_del_tail(struct list_head *head)
{   
	if(!list_empty(head)) {
		list_del(head->prev);
	}
}
	
#endif
