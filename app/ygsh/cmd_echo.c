

#include "fs.h"

int cmd_echo(const char *data, const char *path)
{
    int fd;
    
    fd = ygopen(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    } 

    //只是读取一次数据
    int nbyteswrite = ygwrite(fd, (void*)data, strlen(data));


    ygclose(fd);

    return 0;
}

int echo(const char *data, const char *path)
{
    return cmd_echo(data, path);
}
