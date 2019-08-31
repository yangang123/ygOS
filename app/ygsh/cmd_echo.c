

#include "fs.h"

int cmd_echo(const char *data, const char *path)
{
    int fd;
    
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    } 

    //只是读取一次数据
    int nbyteswrite = write(fd, data, strlen(data));


    close(fd);

    return 0;
}

int echo(const char *data, const char *path)
{
    return cmd_echo(data, path);
}
