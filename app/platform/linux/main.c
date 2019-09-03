

#include <stdio.h>
#include "stdio.h"
#include "rtos.h"
#include "cmd.h"
#include "fatfs.h"


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

int main(int argc, char **argv)
{   
    ygos_init();
    ygos_inode_list_init();
    ramlog_register();
    
    if (!mount(NULL, "/proc", "procfs", 0, NULL)) {
        printf("mount ok!\n");
    }
    
    fat32_test();

	return 0;
}
