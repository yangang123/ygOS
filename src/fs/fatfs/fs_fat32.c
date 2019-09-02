
#include "fatfs.h"

int     fat_open( struct file *filep,  const char *relpath,
        int oflags, mode_t mode);
int     fat_close( struct file *filep);
int     fat_read( struct file *filep,  char *buffer, int buflen);
int     fat_bind( struct inode *blkdriver,  const void *data,
        void **handle);
int     fat_unbind( void *handle,  struct inode **blkdriver,
        unsigned int flags);

//全局文件描述符号
struct fat_file_s g_file;

//全局挂载点
struct fat_mountpt_s g_fat;

const struct mountpt_operations fat_operations =
{
    .open   =fat_open,
    .close  =fat_close,
    .read   =fat_read,
    .write  =NULL,
    .seek   =NULL,
    .ioctl  =NULL,

    .sync   =NULL,
    .dup    =NULL,

    .bind   =fat_bind,
    .unbind =fat_unbind
};

//读取cache数据
int fat_fscacheread(struct fat_mountpt_s *fs, off_t sector)
{
    int ret;
    DEBUG_LR("read fs:%d sector:%d", fs->fs_currentsector, sector);
    if (fs->fs_currentsector != sector)
    {  
        ret = fat_hwread(fs, fs->fs_buffer, sector, 1);
        if (ret < 0)
        {
            return ret;
        }
        fs->fs_currentsector = sector;
    }

    return 0;
}

//跳到下1个目录项
int fat_nextdirentry(struct fat_mountpt_s *fs, struct fs_fatdir_s *dir)
{
    unsigned int ndx;

    ndx = dir->fd_index + 1;
    if ((ndx & (DIRSEC_NDIRS(fs)-1)) == 0)
    {
        dir->fd_currsector++;
    }
    dir->fd_index  = ndx;

    return 0;
}

//查找目录项
static int fat_findsfnentry(struct fat_mountpt_s *fs,
                            struct fat_dirinfo_s *dirinfo)
{   
    //先查找根目录的目录项内容
    int cluster = fs->fs_rootbase;
    DEBUG_LR("fs->fs_rootbase:%d", fs->fs_rootbase);
    dirinfo->dir.fd_startcluster = cluster;
    dirinfo->dir.fd_currcluster  = cluster;
    dirinfo->dir.fd_currsector   = fat_cluster2sector(fs, cluster);
    dirinfo->dir.fd_index = 0;

    for (;;) {
        //读取缓冲区内容
        int ret = fat_fscacheread(fs, dirinfo->dir.fd_currsector);
        if (ret < 0 ) {
            DEBUG_LR("fs->fs_rootbase read, error");
            return -1;
        }

        DEBUG_LR("fd_index:%d", dirinfo->dir.fd_index);
        int direntry_offset = dirinfo->dir.fd_index *32;
        struct  fat32_direntry *dir  = 
            (struct  fat32_direntry *)&fs->fs_buffer[direntry_offset];

        //目录项第一个字符是空，表示没有目录项
        if (dir->name[0] == DIR0_EMPTY) {
            DEBUG_LR("dir end");
            return -1;
        }
        
        //仅仅顶层文件目录项
        if ( ((dir->attr & 8) == 0) & ((dir->attr & 4) == 0) & ((dir->attr & 2) == 0) ) {
            if ( !memcmp(dir->name, dirinfo->fd_name, 11) ) {
                dirinfo->fd_seq.ds_sector      = fs->fs_currentsector;
                dirinfo->fd_seq.ds_offset      = direntry_offset % 512; //32
                DEBUG_LR("found dir");
                return 0;
            }
        }
         
        //遍历下1个目录项
        fat_nextdirentry(fs, &dirinfo->dir);
    }
    
    return -1;
}

//初始化dir的名字
int  parse_path_name(const char *path, struct fat_dirinfo_s *dirinfo)
{   
    char *name = (char*)dirinfo->fd_name;
   
    const char *next_path = path;
    while(*next_path) {
        //跳过‘'’
        if (*next_path == '.') {
           name = (char*)&dirinfo->fd_name[8];
           next_path++;
           continue;
        }
        
        //文件名小写转化成大写
        *name = *next_path; 
        *name = toupper(*name);

        name++;
        next_path++;
    }

    return 0;
}


int     fat_open( struct file *filep,  const char *relpath,
        int oflags, mode_t mode)
{   
    struct inode *inode;
    struct fat_mountpt_s *fs;

    inode = filep->f_inode;
    fs   = inode->i_private;
    struct fat_dirinfo_s dir_info; 
    memset(&dir_info, 0x00, sizeof(struct fat_dirinfo_s));
    memset(dir_info.fd_name, ' ', 11);

    int ret = parse_path_name(relpath, &dir_info);
    if (ret < 0) {
        DEBUG_LR("parse_path_name filed");
    }

    ret = fat_findsfnentry(fs, &dir_info);
    if (ret < 0 ) {
        DEBUG_LR("no find directory entry");
        return -1;
    }

    struct  fat32_direntry* dir = 
        (struct  fat32_direntry*)&fs->fs_buffer[dir_info.fd_seq.ds_offset];
    g_file.ff_startcluster = (dir->cluster_hi << 16) | dir->cluster_lo;
    g_file.ff_currentcluster = g_file.ff_startcluster;
    g_file.ff_size = dir->size;
    DEBUG_LR("g_file.ff_startcluster:%d", g_file.ff_startcluster);
    DEBUG_LR("g_file.ff_size:%d", g_file.ff_size);
    filep->f_priv = &g_file;

    return 0;
}

int     fat_close( struct file *filep)
{
		return 0;
} 

int fat_read( struct file *filep,  char *buffer, int buflen)
{   
    struct inode *inode;
    struct fat_mountpt_s *fs;

    inode = filep->f_inode;
    fs   = inode->i_private;
    DEBUG_LR("fat_read private:%x", fs);
    struct fat_file_s* fat_file = (struct fat_file_s*)filep->f_priv;
    
    //限制文件长度
    if (fat_file->ff_size >= 512 || filep->f_pos >= 512) {
        return -1;
    }

    // 读取长度不能超过文件打长度
    int valid_len = fat_file->ff_size - filep->f_pos;
    if (buflen >  valid_len) {
        buflen = valid_len; 
    }
    
    if(!fat_file->ff_bflags) {
        DEBUG_LR("fat read data");
        uint32_t sector = fat_cluster2sector(fs, fat_file->ff_currentcluster);
        fat_hwread(fs, fat_file->ff_buffer, sector, 1);
        fat_file->ff_bflags = 1;
    }
    memcpy(buffer, &fat_file->ff_buffer[filep->f_pos], buflen);
    DEBUG_LR("ch:%c, buflen:%d", fat_file->ff_buffer[0], buflen);
    filep->f_pos += buflen;

    return buflen;
} 

int fat_bind( struct inode *blkdriver,  const void *data,
            void **handle)
{   
    fat_mount(&g_fat);
    *handle = (void *)&g_fat;
    return 0;
} 

int fat_unbind( void *handle,  struct inode **blkdriver,
        unsigned int flags)
{
		return 0;
} 

