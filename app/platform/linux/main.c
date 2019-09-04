

#include <ygos/libc/stdio.h>
#include <ygos/fs/fatfs/fatfs.h>
#include <ygos/queue.h>
#include <ygos/rtos.h>
#include <ygos/mm/mm.h>
#include "cmd.h"

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

void mem_test(void)
{
    
    for (int i = 0; i < 6; i++) {
       void *p= ygos_malloc(1<<(4+i));   
       printf("i: %d, p:%x\n",1<<(4+i), p);
       ygos_free(p);
    }
}

int main(int argc, char **argv)
{   
    ygos_mem_init();
    ygos_init();
    ygos_inode_list_init();
    ramlog_register();
    
    if (!mount(NULL, "/proc", "procfs", 0, NULL)) {
        printf("mount ok!\n");
    }

    fat32_test();

	return 0;
}
