#include "list.h"
#include "queue.h"

#include <stdint.h>
#include "debug.h"

# define MM_ALLOC_BIT    0x80000000
# define SIZEOF_MM_ALLOCNODE    8

typedef unsigned int mmsize_t;

struct mm_freenode_s
{
    mmsize_t size;                   /* Size of this chunk */
    mmsize_t preceding;              /* Size of the preceding chunk */
    struct mm_freenode_s *next;     /* Supports a doubly linked list */
};

struct mempool{
      uint32_t per_chunk_size;
      uint8_t  chunk; 
      void*    block;
      yg_sq_queue_t  free_queue; 
};

uint8_t mempool_1[10][16+SIZEOF_MM_ALLOCNODE]; //24
uint8_t mempool_2[10][32+SIZEOF_MM_ALLOCNODE]; //40 
uint8_t mempool_3[10][64+SIZEOF_MM_ALLOCNODE]; //72
uint8_t mempool_4[10][128+SIZEOF_MM_ALLOCNODE]; // 136

struct mm_heap_s
{   
    struct mempool   mempool_list[4];
};

struct mm_heap_s g_kmm_heap  = {
    .mempool_list = {
        {16+SIZEOF_MM_ALLOCNODE, 10,    mempool_1},
        {32+SIZEOF_MM_ALLOCNODE, 10,    mempool_2},
        {64+SIZEOF_MM_ALLOCNODE, 10,    mempool_3},
        {128+SIZEOF_MM_ALLOCNODE, 10,   mempool_4}
    }
};

static void mem_pool_init(struct mempool *pool)
{   
    sq_init(&pool->free_queue);

    //初始化多少块, &kmm
    for (int i = 0 ; i < pool->chunk; i++) { 
        uint32_t offset = pool->chunk*i;
        DEBUG_LR("mem offset:%d", offset);
        yg_sq_add_tail(&pool->free_queue,(yg_sq_entry_t *)&pool->block[offset]); 
    }
}

void mem_init(struct mm_heap_s *kmm_heap)
{
    for (int i = 0 ; i < 1; i++) {
        mem_pool_init(&g_kmm_heap.mempool_list[i]);
    }
}


