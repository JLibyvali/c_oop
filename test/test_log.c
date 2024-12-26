#include "debug.h"

#include <stdio.h>

int   infunc() { return -1; }

void  func() { Checkret(infunc(), -1, , "infunc() retrun error"); }

char *color() { return LOG_RED; }

int   main()
{
    printf("Hello\n");

    // test log_level type check;
    func();
    // test return a color code;
    char *col = color();
    printf("%sTest Return Color Value\n" LOG_NONE, col);

    // Test Success write to logfile?
    int err = -1;
    Error("Test Write File Code: %d\n", err);

    return 0;
}