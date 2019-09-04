#ifndef _MM_H
#define _MM_H

#include <ygos/libc/stdint.h>
#include <ygos/queue.h>

#define MM_MIN_SHIFT            4                   
#define MM_MIN_CHUNK            (1 << MM_MIN_SHIFT) //内存池最小块大小是16字节
#define MM_MIN_ALIGN_MASK       (MM_MIN_CHUNK-1)
#define MM_ALIGN_UP(a)          (((a) + MM_MIN_ALIGN_MASK) & ~MM_MIN_ALIGN_MASK)

#define MM_USED_BIT             0x80000000          //内存块使用标志
#define SIZEOF_MM_ALLOCNODE     8                   //内存块头大小

#ifndef MEM_POOL_NUM
#define MEM_POOL_NUM            6                   //内存池类型的大小
#endif 

#define MEM_OVERFLLOW_CHECK_VALUE  0xED             //内存初始值

#define MM_ADD_NODE_ADDR(a)     (a+16)

union mm_freenode_u
{    
    union mm_freenode_u*   next;                   //next指针放到结构体第一个成员的位置
    struct desc_s {
        uint32_t           size;                   //内存大小    
        uint32_t           flag;                   //内存使用标记
    } mem_head;
};

struct mempool{
    uint32_t per_chunk_size;                        //内存池
    uint8_t  chunk;                                 //内存池内存块的数量
    void*    block;                                 //内存池内存地址
    yg_sq_queue_t  free_queue;                      //内存池空闲链表队列
};

struct mm_heap_s
{   
    struct mempool   mempool_list[MEM_POOL_NUM];    //内存池定义
};

void    ygos_mem_init(void);
void*   ygos_malloc(int size);
int     ygos_free(void *ptr);

#endif /* _MM_H */
