

#include <stdio.h>
#include "stdio.h"
#include "rtos.h"
#include "cmd.h"
#include "fatfs.h"
#include "queue.h"

void inode_handler(inode_t* node, struct inode_path_s *info)
{
    DEBUG("name:%s\n",  info->path);
}

void inode_test(void)
{   
    struct inode_path_s info;
    memset(&info.path, 0x00, sizeof(info.path));
    info.handler = inode_handler;
    ygos_inode_foreach(g_root_inode, &info);
}

void ramlog_test(void)
{    
    echo("log", "/dev/ramlog");
    cat("/dev/ramlog");
}

void fat32_test(void)
{    
    if (!mount(NULL, "/fat", "fatfs", 0, NULL)) {
        printf("fatfs mount ok!\n");
    }

    cat("/fat/test1.c");
}

void proc_test()
{   
    cat("/proc/uptime");
}



// void sq_add_first(yg_sq_queue_t*queue, yg_sq_entry_t *node)
// {   
//     node->next = queue->head;
//     if (!queue->head) {
//         queue->tail = node;
//         queue->head = node;
//     } else {
//         queue->head = node;
//     }
// }



struct name{
    struct name *next; 
    int number;
};
yg_sq_queue_t name_queue;

struct name table[5] = {
    {NULL, 1},
    {NULL, 2},
    {NULL, 3},
    {NULL, 4},
    {NULL, 5}
};

void test(void)
{  
    sq_init(&name_queue);

    for (int i = 0; i < 5; i++) {
        yg_sq_add_tail(&name_queue, (yg_sq_entry_t *)&table[i]);
    }

    for (int i = 0; i < 5; i++) {
        yg_sq_remove_first(&name_queue);
    }
}
void mem_init(void);

int main(int argc, char **argv)
{   
    mem_init();

    for (int i = 1; i < 2; i+=5) {
       void *p= yg_malloc(i);   
       printf("i: %d, p:%x\n",i, p);
       yg_free(p);
    }

    // ygos_init();
    // ygos_inode_list_init();
    // ramlog_register();
    
    // if (!mount(NULL, "/proc", "procfs", 0, NULL)) {
    //     printf("mount ok!\n");
    // }

    
    
    // fat32_test();

	return 0;
}
