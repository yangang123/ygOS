
#include "fatfs.h"
#include "fs.h"

extern struct fat_mountpt_s g_fat;
uint8_t fat_buffer[512];

//挂载文件系统
void fat_mount(struct fat_mountpt_s *fs)
{   
    fs->fs_buffer = fat_buffer;
    fs->fd =  -1;
    int nsector = fat_hwread(fs,fs->fs_buffer, 0, 1);
    if (nsector != 1) {
        return;
    }
    fs->fs_currentsector = 0;

    g_fat.fb = (struct	fat_boot*)fs->fs_buffer;
    fs->fs_fatnumfats =2 ;
    fs->fs_hwsectorsize   =     fs->fb->bytes_per_sector;
    fs->fs_hwnsectors =         fs->fb->total_sectors; 
    fs->fs_fatresvdseccount   = fs->fb->reserved_sectors;
    fs->fs_fattotsec  = fs->fb->total_sectors;
    fs->fs_database   = fs->fb->reserved_sectors + fs->fb->fat32.sectors_per_fat * 2;
    fs->fs_fatbase    = fs->fb->reserved_sectors;
    
    fs->fs_rootentcnt = 0;
    fs->fs_fatnumfats = 2;
    fs->fs_nfatsects  = fs->fb->fat32.sectors_per_fat;
    //int ntotalfatsects = fs->fs_fatnumfats * fs->fs_nfatsects;
    fs->fs_fatsecperclus  = fs->fb->sectors_per_cluster;
    //int ndatasectors    = fs->fs_fattotsec - fs->fs_fatresvdseccount - ntotalfatsects;
    fs->fs_rootbase     = fs->fb->fat32.root_cluster;
    fs->fs_type = 32;

    DEBUG_LR("FAT%d:\n", fs->fs_type = 32);
    DEBUG_LR("\tHW  sector size:     %d", fs->fs_hwsectorsize);
    DEBUG_LR("\t    sectors:         %d", fs->fs_hwnsectors);
    DEBUG_LR("\tFAT reserved:        %d", fs->fs_fatresvdseccount);
    DEBUG_LR("\t    sectors:         %d", fs->fs_fattotsec);
    DEBUG_LR("\t    start sector:    %d", fs->fs_fatbase);
    DEBUG_LR("\t    root sector:     %d", fs->fs_rootbase); 
    DEBUG_LR("\t    root entries:    %d", fs->fs_rootentcnt);
    DEBUG_LR("\t    data sector:     %d", fs->fs_database);
    DEBUG_LR("\t    Num FATs:        %d", fs->fs_fatnumfats);
    DEBUG_LR("\t    FAT sectors:     %d", fs->fs_nfatsects);
    DEBUG_LR("\t    sectors/cluster: %d", fs->fs_fatsecperclus);
}

//簇转化成扇区
off_t fat_cluster2sector( struct fat_mountpt_s *fs,  uint32_t cluster)
{
  cluster -= 2;
  
  return cluster * fs->fs_fatsecperclus + fs->fs_database;
}
