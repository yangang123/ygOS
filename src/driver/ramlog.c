
#include <ygos/fs/fs.h>
#include <ygos/driver/ramlog.h>

char  g_ramlog_buf[RAMLOG_BUF_SIZE_MAX] = {0x00};

static int  ramlog_open( struct file *filep);
static int  ramlog_close( struct file *filep);
static int  ramlog_read( struct file *filep,  char *buffer,
                               int buflen);
static int  ramlog_write( struct file *filep,
                                 const char *buffer, int buflen);
static int  ramlog_ioctl( struct file *filep, int cmd,
                                unsigned long arg);

static const struct file_operations g_ramlogops =
{
    ramlog_open,  /* open */
    ramlog_close, /* close */
    ramlog_read,  /* read */
    ramlog_write, /* write */
    ramlog_ioctl  /* ioctl */
};

static int  ramlog_open( struct file *filep)
{   
    DEBUG("ramlog_open\n");
    return 0;
}

static int  ramlog_close( struct file *filep)
{
    return 0;
}

static int  ramlog_read( struct file *filep,  char *buffer,
                               int buflen)
{   
    /*ram log是一个字符串缓冲区*/
    int len = 0;
    if(buflen < RAMLOG_BUF_SIZE_MAX) {
        len = buflen; 
    } else {
        len = RAMLOG_BUF_SIZE_MAX;
    }
    
    memcpy(buffer, g_ramlog_buf, len);   
    return strlen(g_ramlog_buf);
}

static int  ramlog_write( struct file *filep,
                                 const char *buffer, int buflen)
{   
    /* ramlog 写数据到全局buffer中 */
    int len = 0;
    if(buflen < RAMLOG_BUF_SIZE_MAX) {
        len = buflen; 
    } else {
        len = RAMLOG_BUF_SIZE_MAX;
    }
    
    memcpy(g_ramlog_buf, buffer, len);   
    DEBUG("ramlog_write:%s\n", g_ramlog_buf);
    return len;
}

static int  ramlog_ioctl( struct file *filep, int cmd,
                                unsigned long arg)
{
    return 0;
}

//注册ramlog驱动
void ramlog_register(void)
{
     register_driver("/dev/ramlog", &g_ramlogops, (void*)0);
}
