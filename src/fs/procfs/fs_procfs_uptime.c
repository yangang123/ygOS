
#include "procfs.h"
#include "rtos.h"

struct uptime_file_s g_uptime_file;

static int uptime_open( struct file *filep,  const char *relpath,
                int oflags, mode_t mode)
{
    if (strcmp(relpath, "uptime") != 0)
    {   
        return -1;
    }
    filep->f_priv = (void*)&g_uptime_file;

    DEBUG_LR("relpath:%s", relpath);
    return 0;
}

static int uptime_close( struct file *filep)
{
    filep->f_priv = NULL;

    return 0;
}

static int uptime_read( struct file *filep,  char *buffer, size_t buflen)
{  
    struct uptime_file_s *attr;

    attr = filep->f_priv;
   
    int tick = ygos_get_tick();

    sprintf(attr->line, "%d", tick);
    int len = strlen(attr->line);
    memcpy(buffer, attr->line, len);
    DEBUG_LR("attr->line:%s", attr->line);
    return len;
}

const struct procfs_operations  uptime_operations = 
{
    uptime_open,
    uptime_close,
    uptime_read,
};
