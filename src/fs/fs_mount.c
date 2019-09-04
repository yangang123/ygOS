
#include <ygos/fs/fs.h>

struct fsmap_t
{
   const char                      *fs_filesystemtype;
   const struct mountpt_operations *fs_mops;
};

extern const struct mountpt_operations procfs_operations;
extern const struct mountpt_operations fat_operations;

static const struct fsmap_t ygos_g_fs_table[] =
{
    { "procfs", &procfs_operations },
    { "fatfs", &fat_operations },
    { NULL, NULL },
};

//查找文件系统是否存在，全局表中
//fs： 最后1个是空指针结束
static const struct mountpt_operations* ygos_mount_find_fs(const struct fsmap_t *fs, 
    const char *fs_type)
{   
    const struct fsmap_t *fs_i;
    for (fs_i = fs; fs_i->fs_filesystemtype; fs_i++) {
        DEBUG_LR("fs:%s obj:%s", fs->fs_filesystemtype, fs_type);
        if (0 == strcmp(fs_i->fs_filesystemtype, fs_type))  {
            return fs_i->fs_mops;
        }
    }
    return NULL;
}

//挂载文件系统
// source: 块设备节点 target: 挂载路径 filesystemtype：文件系统路径
// mountflags: 挂载属性 data：传递数据
int mount( const char *source,  const char *target,
           const char *filesystemtype, unsigned long mountflags,
           const void *data)
{   
    
    DEBUG_LR("mount");

    // 查找当前挂载系统是否存在表中
    const struct mountpt_operations *fs_mops;
    fs_mops= ygos_mount_find_fs(ygos_g_fs_table,filesystemtype); 
    if (!fs_mops) {
        DEBUG_LR("fs_mops is null");
        return -1;
    }
    
    // 获取挂载点
    inode_t *mountpt_inode;
    int ret = -1;
    ret = ygos_inode_get(target, &mountpt_inode);
    if (ret < 0) {
        DEBUG_LR("mountpt_inode is null");
        return -1;
    }
    
    // 设备点挂载驱动， 设置挂载点属性
    void *fshandle;
    ret = fs_mops->bind(NULL, data, &fshandle);
    FILE_NODE_TYPE_SET_MOUNTPT(mountpt_inode);
    mountpt_inode->u.i_mops  = fs_mops;
    mountpt_inode->i_private = fshandle;
    DEBUG_LR("mount ok i_priv:%x",mountpt_inode->i_private);
    return 0;
}
