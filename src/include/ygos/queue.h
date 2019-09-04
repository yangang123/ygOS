#ifndef _QUEUE_H
#define _QUEUE_H

#define NULL ((void*)0)

struct yg_sq_entry_s
{
   struct yg_sq_entry_s *next;
};
typedef struct yg_sq_entry_s yg_sq_entry_t;

struct yg_sq_queue_s
{
   yg_sq_entry_t *head;
   yg_sq_entry_t *tail;
};
typedef struct yg_sq_queue_s  yg_sq_queue_t;

//初始化队列
static inline void sq_init(yg_sq_queue_t*queue)
{   
    queue->head = NULL;
    queue->tail = NULL;
}

//移除队列中的第一个
static inline yg_sq_entry_t *yg_sq_remove_first(yg_sq_queue_t*queue)
{   
    yg_sq_entry_t *node;
    
    //取出头节点
    node = queue->head;
    if (!queue->head) {
       return NULL;
    }
    
    //将头指针的下一个节点，作为头节点
    queue->head = queue->head->next;
    if( !queue->head ) {
        queue->tail = NULL;
    }

    node->next = NULL;
    return node;
}

//添加节点到队尾
static inline void yg_sq_add_tail(yg_sq_queue_t*queue, yg_sq_entry_t *node)
{   
    node->next = NULL;
    if (!queue->head) {
        //第一次
        queue->head = node;
        queue->tail = node;
    } else {
        
        //当前尾指针的next指向下一个节点，移动尾指针到下一个节点
        queue->tail->next = node;
        queue->tail = node;  
    }
}

//添加节点到队头
static inline void yg_sq_add_first(yg_sq_queue_t*queue, yg_sq_entry_t *node)
{   
    node->next = queue->head;
    if (!queue->head) {
        queue->tail = node;
        queue->head = node;
    } else {
        queue->head = node;
    }
}

#endif
