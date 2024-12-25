#include "debug.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GEN_LOGFILE 1

int main()
{
    printf("Hello\n");
    Error("Message From Error");

    return 0;
}