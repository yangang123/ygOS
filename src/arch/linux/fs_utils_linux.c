
#include <ygos/fs/fatfs/fatfs.h>

#define LINUX_SD_DEVICE_NODE_NAME "/dev/sdc"

//读1个扇区数据
int fat_hwread(struct fat_mountpt_s *fs, uint8_t *buffer,  int sector,
               unsigned int nsectors)
{  
    int ret= -1;
    // 打开设备节点
    if (fs->fd < 0) {
        fs->fd  = open(LINUX_SD_DEVICE_NODE_NAME, O_RDWR);
        if (fs->fd <0 ) {
            DEBUG_LR("open error");
            return -1;
        }
    }
    
    //读取某个扇区的数据
    int offset = sector * 512;
    lseek(fs->fd, offset, SEEK_SET);
    int len = read(fs->fd, buffer, 512*nsectors);
    
    DEBUG_LR("sector: %d, len:%d", sector, len);
    for (uint8_t i = 0; i < 10; i ++) {
        DEBUG_COMMON("%x,", buffer[i]);
    }
    DEBUG_COMMON("\n");

    return len/512;
}

//写1个扇区数据
int fat_hwwrite(struct fat_mountpt_s *fs, uint8_t *buffer, off_t sector,
                unsigned int nsectors)
{   
    int ret = -1;
    return ret;
}


