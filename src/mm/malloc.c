#include <ygos/list.h>
#include <ygos/queue.h>
#include <ygos/debug.h>
#include <ygos/mm/mm.h>

static  uint8_t mempool_1[10][MM_ADD_NODE_ADDR(16)]; 
static  uint8_t mempool_2[10][MM_ADD_NODE_ADDR(32)]; 
static  uint8_t mempool_3[10][MM_ADD_NODE_ADDR(64)]; 
static  uint8_t mempool_4[10][MM_ADD_NODE_ADDR(128)]; 
static  uint8_t mempool_5[10][MM_ADD_NODE_ADDR(256)]; 
static  uint8_t mempool_6[10][MM_ADD_NODE_ADDR(512)]; 

static  struct mm_heap_s g_kmm_heap  = {
    .mempool_list = {
        {MM_ADD_NODE_ADDR(16),    10,     mempool_1},
        {MM_ADD_NODE_ADDR(32),    10,     mempool_2},
        {MM_ADD_NODE_ADDR(64),    10,     mempool_3},
        {MM_ADD_NODE_ADDR(128),   10,     mempool_4},
        {MM_ADD_NODE_ADDR(256),   10,     mempool_5},
        {MM_ADD_NODE_ADDR(512),   10,     mempool_6},
    }
};

//内存池初始化
static void ygos_mem_pool_init(struct mempool *pool)
{   
    sq_init(&pool->free_queue);

    //初始化多少块, &kmm
    for (int i = 0 ; i < pool->chunk; i++) { 
        uint32_t offset = pool->per_chunk_size*i;
		uint8_t *buffer = (uint8_t *)pool->block;
        yg_sq_add_tail(&pool->free_queue,(yg_sq_entry_t *)&buffer[offset]);
        DEBUG_LR("mem addr:0x%x", (yg_sq_entry_t *)&buffer[offset]);   
    }
}

//内存初始化所有内存池
void ygos_mem_init(void)
{
    for (int i = 0 ; i < 6; i++) {
        ygos_mem_pool_init(&g_kmm_heap.mempool_list[i]);
        DEBUG_LR("\n\n");
    }
}

//内存分配
void* ygos_malloc(int size)
{
    if (size < 1) {
      return NULL;
    }
    
    //查找所有内存池打内存块大小是否接近当前申请空间
    int  alignsize = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);
    DEBUG_LR("alignsize:%d", alignsize);
    int index = -1;
    for (int i = 0; i < MEM_POOL_NUM; i++) {
        if (alignsize <= g_kmm_heap.mempool_list[i].per_chunk_size) {
            //找到第一个大于size的空间
            index = i;
            break;
        }
    }
    if (index < 0 ||  index >= MEM_POOL_NUM) {
        return NULL;
    }
    //内存池从空闲链表分配内存
    void *ret = NULL;
    union mm_freenode_u *node = NULL;
    node = (union mm_freenode_u *)yg_sq_remove_first(&g_kmm_heap.mempool_list[index].free_queue);
    if (node) {
        node->mem_head.flag  = MM_USED_BIT;
        node->mem_head.size  = g_kmm_heap.mempool_list[index].per_chunk_size;
        
        //跳过内存会头部在地址
        ret         = (char*)node + SIZEOF_MM_ALLOCNODE;
        
        //内存溢出检查，如果新分配的内存地址初始值不是MEM_OVERFLLOW_CHECK_VALUE，那么内存溢出
        memset(ret, MEM_OVERFLLOW_CHECK_VALUE, node->mem_head.size-SIZEOF_MM_ALLOCNODE);
        DEBUG_LR("pool index:%d, node addr:0x%x, node size:%d, base:0x%x", index, (void*)node, alignsize, ret);
    }

    return ret;
}

//内存释放
int ygos_free(void *ptr)
{
    if (!ptr) {
      return NULL;
    }

    //查找释放内存属于那个内存池
   union mm_freenode_u*node;
    node = (union mm_freenode_u*)(( char *)ptr - SIZEOF_MM_ALLOCNODE);
    DEBUG_LR("ptr:%x node:%x, node.size:%d, node_flag:0x%x", (void *)ptr, node, node->mem_head.size, node->mem_head.flag);
    int index = -1;
    for (int i = 0; i < MEM_POOL_NUM; i++) {
        //找到第一个大于size的空间
        if (node->mem_head.size <= g_kmm_heap.mempool_list[i].per_chunk_size) {
            index = i;
            break;
        }
    }
    if (index < 0 ||  index >= MEM_POOL_NUM) {
        return NULL;
    }

    //清除内存占用标志
    node->mem_head.flag &= ~MM_USED_BIT;
    //释放内存快，自动添加到内存块空闲对应在链表
    yg_sq_add_tail(&g_kmm_heap.mempool_list[index].free_queue, (yg_sq_entry_t *)node);
    DEBUG_LR("pool index:%d, node addr:0x%x", index, (void*)node);

    return 0;
}
