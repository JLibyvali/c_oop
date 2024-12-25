#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


// Generate log file
static int is_loginit = 0;
FILE      *log_ptr;

void       gen_logfile(void)
{
    if (is_loginit)
        return;

    if (ISDEF(GEN_LOGFILE))
    {
        char name[128];
        snprintf(name, 128, "./log/c_oop.log.%d", getpid());
        log_ptr = fopen(name, "wb+");
        if (log_ptr == NULL)
            Panic("Generate Log File FAILED!!");
        is_loginit = 1;
    }
    return;
}