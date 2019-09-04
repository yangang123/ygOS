#include "list.h"
#include "queue.h"

#include <stdint.h>
#include "debug.h"

# define MM_ALLOC_BIT    0x80000000
# define SIZEOF_MM_ALLOCNODE    8

#define MEM_POOL_NUM    4

typedef unsigned int mmsize_t;

struct mm_freenode_s
{  
    struct mm_freenode_s *next;  //next指针放到结构体第一个成员的位置
    mmsize_t size;               //内存大小    
    mmsize_t flag;                  //标记内存是否使用
};

struct mempool{
      uint32_t per_chunk_size;
      uint8_t  chunk; 
      void*    block;
      yg_sq_queue_t  free_queue; 
};

uint8_t mempool_1[10][16]; //24
uint8_t mempool_2[10][32]; //40 
uint8_t mempool_3[10][64]; //72
uint8_t mempool_4[10][128]; // 136

struct mm_heap_s
{   
    struct mempool   mempool_list[MEM_POOL_NUM];
};

struct mm_heap_s g_kmm_heap  = {
    .mempool_list = {
        {16, 10,    mempool_1},
        {32, 10,    mempool_2},
        {64, 10,    mempool_3},
        {128, 10,   mempool_4}
    }
};

static void mem_pool_init(struct mempool *pool)
{   
    sq_init(&pool->free_queue);

    //初始化多少块, &kmm
    for (int i = 0 ; i < pool->chunk; i++) { 
        uint32_t offset = pool->per_chunk_size*i;
        yg_sq_add_tail(&pool->free_queue,(yg_sq_entry_t *)&pool->block[offset]);
        printf("mem addr:0x%x\n", (yg_sq_entry_t *)&pool->block[offset]);   
    }
}
#define MM_MIN_CHUNK     (1 << MM_MIN_SHIFT)
#define MM_MAX_CHUNK     (1 << MM_MAX_SHIFT)
#define MM_NNODES        (MM_MAX_SHIFT - MM_MIN_SHIFT + 1)

#define MM_GRAN_MASK     (MM_MIN_CHUNK-1)

#  define MM_MIN_SHIFT    4  /* 16 bytes */
#define MM_ALIGN_UP(a)   (((a) + MM_GRAN_MASK) & ~MM_GRAN_MASK)

#define MM_USED   1

void mem_init(void)
{
    for (int i = 0 ; i < 2; i++) {
        mem_pool_init(&g_kmm_heap.mempool_list[i]);
        printf("\n\n");
    }
}

void* yg_malloc(size_t size)
{
    if (size < 1)
    {
      return NULL;
    }
    size_t  alignsize = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);
    DEBUG_LR("alignsize:%d", alignsize);
    
    int index = -1;
    for (int i = 0; i < MEM_POOL_NUM; i++) {
        //找到第一个大于size的空间
        if (alignsize <= g_kmm_heap.mempool_list[i].per_chunk_size) {
            index = i;
            break;
        }
    }
    if (index < 0 ||  index >= MEM_POOL_NUM) {
        return NULL;
    }

    DEBUG_LR("index:%d", index);
    void *ret = NULL;
    struct mm_freenode_s *node = NULL;
    node = yg_sq_remove_first(&g_kmm_heap.mempool_list[index].free_queue);
    if (node) {
        DEBUG_LR("node addr:%x", node);
        node->flag  = MM_USED;
        node->size  = g_kmm_heap.mempool_list[index].per_chunk_size;
       
        ret         = (char*)node + SIZEOF_MM_ALLOCNODE;
    }

    return ret;
}


void* yg_free(void *mem)
{
    if (!mem)
    {
      return NULL;
    }
    struct mm_freenode_s *node;

    node = ( struct mm_freenode_s *)(( char *)mem - SIZEOF_MM_ALLOCNODE);
    DEBUG_LR("node:%x, node.size:%d, node_flag:%d", node, node->size, node->flag);

    node->flag &= ~MM_UNUSE;
    
    //hash算法,整数
    
    int index = -1;
    for (int i = 0; i < MEM_POOL_NUM; i++) {
        //找到第一个大于size的空间
        if (node->size <= g_kmm_heap.mempool_list[i].per_chunk_size) {
            index = i;
            break;
        }
    }
    if (index < 0 ||  index >= MEM_POOL_NUM) {
        return NULL;
    }

    DEBUG_LR("index:%d", index);
    yg_sq_add_tail(&g_kmm_heap.mempool_list[index].free_queue, (yg_sq_entry_t *)node);

    return 0;
}
