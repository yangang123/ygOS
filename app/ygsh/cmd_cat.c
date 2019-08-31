

#include "fs.h"
#include "cmd.h"

typedef void (*cmd_handler_t)(char *data, int len, int end);

static char cat_buffer[CAT_BUFFER_SIZE];
char g_cat_buffer[CAT_BUFFER_SIZE+1] ={0x00};

static void defalut_print_handler(char *data, int len, int end)
{  
    int retlen = snprintf(g_cat_buffer, CAT_BUFFER_SIZE,"%s", data);
    g_cat_buffer[retlen] = '\0';
#ifdef STD_OUT
    printf(g_cat_buffer);
    if (end) {
        putchar('\n');
    }
#endif
}

static int cmd_cat(cmd_handler_t handler, const char *path)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    } 
    
    //只是读取一次数据
    int nbytesread = read(fd, cat_buffer, CAT_BUFFER_SIZE);
    if (nbytesread < 0) {
        return -1;
    }
    handler(cat_buffer, nbytesread, 1);

   close(fd);

    return 0;
}


void cat(const char *path)
{
    cmd_cat(defalut_print_handler, path);
}

