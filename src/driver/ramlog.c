

#include "fs.h"

static int            can_open( struct file *filep);
static int            can_close( struct file *filep);
static int        can_read( struct file *filep,  char *buffer,
                               int buflen);
static int        can_write( struct file *filep,
                                 const char *buffer, int buflen);
static int            can_ioctl( struct file *filep, int cmd,
                                unsigned long arg);

static const struct file_operations g_canops =
{
  can_open,  /* open */
  can_close, /* close */
  can_read,  /* read */
  can_write, /* write */
  can_ioctl  /* ioctl */
};

static int  can_open( struct file *filep)
{
    return 0;
}

static int  can_close( struct file *filep)
{
    return 0;
}

static int  can_read( struct file *filep,  char *buffer,
                               int buflen)
{
    return 0;
}

static int  can_write( struct file *filep,
                                 const char *buffer, int buflen)
{
    return 0;
}

static int  can_ioctl( struct file *filep, int cmd,
                                unsigned long arg)
{
    return 0;
}


void can_register(void)
{
     register_driver("/dev/ramlog", &g_canops, (void*)0);
}