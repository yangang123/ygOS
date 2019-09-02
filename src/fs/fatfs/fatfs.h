#ifndef _FATFS_H
#define _FATFS_H

#include "fs.h"
#include <stdint.h>
#include <stdbool.h>


#define DIR0_EMPTY         	0xe5 
#define DIR0_ALLEMPTY      	0x00 
#define DIR0_E5            	0x05 
#define DIR_MAXFNAME 		11
#define DIRSEC_NDIRS(f)     (((f)->fs_hwsectorsize) >> 5)

# define toupper(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
# define tolower(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

//FAT32短文件名字目录项
struct fat32_direntry {
	char		name[11];            /*0*/
	uint8_t		attr;					/*11*/
	uint8_t		res;            		/*12*/
	uint8_t		ctime_ms;       		/*13*/
	uint16_t	ctime_time;     		/*14*/
	uint16_t	ctime_date;     		/*16*/
	uint16_t	atime_date;     		/*18*/
	uint16_t	cluster_hi;     		/*20*/
	uint16_t	mtime_time;     		/*22*/
	uint16_t	mtime_date;     		/*24*/
	uint16_t	cluster_lo;     		/*26*/
	uint32_t	size;           		/*28*/
} __attribute__ ((__packed__));
//FAT32内容
struct fat_boot_fat32 {
	uint32_t	sectors_per_fat;		/*36*/    
	uint16_t	fat_flags;              /*40*/
	uint16_t	version;                /*42*/
	uint32_t	root_cluster;           /*44*/
	uint16_t	fsinfo_sector;          /*48*/
	uint16_t	backup_sector;          /*50*/
	uint8_t		reserved2[12];          /*52*/
	uint8_t		drive_number;           /*64*/
	uint8_t		reserved3;              /*65*/
	uint8_t		ext_sig;                /*66*/
	uint32_t	serial;                 /*67*/
	char		label[11];              /*71*/
	char		type[8];                /*82*/
} __attribute__ ((__packed__));

// Boot 扇区
struct fat_boot {
	uint8_t		jmp_boot[3];			/* 0*/
	char		oemname[8];             /* 3*/
	uint16_t	bytes_per_sector;       /*11*/
	uint8_t		sectors_per_cluster;    /*13*/
	uint16_t	reserved_sectors;       /*14*/
	uint8_t		fat_count;              /*16*/
	uint16_t	root_max_entries;       /*17*/
	uint16_t	total_sectors_small;    /*19*/
	uint8_t		media_info;             /*21*/
	uint16_t	sectors_per_fat_small;  /*22*/
	uint16_t	sectors_per_track;      /*24*/
	uint16_t	head_count;             /*26*/
	uint32_t	fs_offset;              /*28*/
	uint32_t	total_sectors;          /*32*/
	struct fat_boot_fat32  fat32;       /*36*/
} __attribute__ ((__packed__));

//fat文件挂载
struct fat_mountpt_s {
	int 		fd;
	int			fs;
	struct		fat_boot *fb;
	off_t    	fs_hwsectorsize;        
	off_t    	fs_hwnsectors;          
	off_t    	fs_fatbase;             
	off_t    	fs_rootbase;            
	off_t    	fs_database;           
	off_t    	fs_fsinfo;             
	off_t    	fs_currentsector;       
	uint32_t 	fs_nclusters;           
	uint32_t 	fs_nfatsects;           
	uint32_t 	fs_fattotsec;           
	uint32_t 	fs_fsifreecount;       
	uint32_t 	fs_fsinextfree;         
	uint16_t 	fs_fatresvdseccount;    
	uint16_t 	fs_rootentcnt;         
	uint8_t  	fs_type;               
	uint8_t  	fs_fatnumfats;          
	uint8_t  	fs_fatsecperclus;       
	uint8_t *	fs_buffer;             								
};

//fat文件描述符
struct fat_file_s
{
	uint8_t  	ff_bflags;            
	uint16_t 	ff_dirindex;            
	uint32_t 	ff_currentcluster;     
	uint16_t    ff_dirsector;          
	uint16_t    ff_size;               
	uint16_t    ff_startcluster;       
	uint16_t    ff_currentsector;      
	uint16_t    ff_cachesector;         
	uint8_t 	ff_buffer[512];              
};

//记录文件打fat表
struct fs_fatdir_s
{
	off_t        fd_startcluster;       
	off_t        fd_currcluster;        
	off_t        fd_currsector;          
	unsigned int fd_index;              
};

//记录目录
struct fat_dirseq_s
{
	uint16_t	ds_offset;             
	off_t		ds_sector;            
};

//记录当前目录状态
struct fat_dirinfo_s
{
	uint8_t 	fd_name[DIR_MAXFNAME];   
	bool     	fd_root;
	struct fat_dirseq_s fd_seq;     
	struct fs_fatdir_s dir;  
};


//簇转化成扇区
off_t fat_cluster2sector( struct fat_mountpt_s *fs,  uint32_t cluster);


//读1个扇区数据
int fat_hwread(struct fat_mountpt_s *fs, uint8_t *buffer,  int sector,
               unsigned int nsectors);

//写1个扇区数据
int fat_hwwrite(struct fat_mountpt_s *fs, uint8_t *buffer, off_t sector,
                unsigned int nsectors);

//块设备挂载到FAT文件系统
void fat_mount(struct fat_mountpt_s *fs);

#endif /*_FATFS_H*/
