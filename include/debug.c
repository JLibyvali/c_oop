#include "debug.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <threads.h>
#include <unistd.h>

// ######################################################################################
//  File of log
// ######################################################################################

static int is_loginit = 0;
FILE      *log_ptr;

void       gen_logfile(void)
{
    if (is_loginit)
        return;

    if (ISDEF(GEN_LOGFILE))
    {
        // Direct to a directory
        int ret = mkdir("./C_OOP.logs", S_IRWXU | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);
        if (ret == -1 && errno != EEXIST)
            Panic("Generate Log File Directory FAILED!!");

        // log file name C_oop.log.PID
        char name[128];
        snprintf(name, 128, "./C_OOP.logs/C_oop.log.%d", getpid());
        log_ptr = fopen(name, "wb+");
        if (log_ptr == NULL)
            Panic("Generate Log File FAILED!!");
        is_loginit = 1;
    }

    return;
}

char *check_level(log_level _level)
{
    switch (_level)
    {
    case INFO:
        {
            return LOG_NONE;
            break;
        }
    case WARN:
        {
            return LOG_PURPLE;
            break;
        }
    default:
        {
            return LOG_RED;
            break;
        }
    };
}
