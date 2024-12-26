#ifndef C_OOP_DEBUG__H
#define C_OOP_DEBUG__H

#include <assert.h>
#include <stdio.h>

// #####################################################################################
//  Debug Macro
// #####################################################################################
#define LOG_RED             "\33[1;31m"
#define LOG_GREEN           "\33[1;32m"
#define LOG_YELLOW          "\33[1;33m"
#define LOG_BLUE            "\33[1;34m"
#define LOG_PURPLE          "\33[1;35m"
#define LOG_CYAN            "\33[1;36m"
#define LOG_WHITE           "\33[1;37m"
#define LOG_NONE            "\33[0m"

// format output string
#define LOG_FMT(str, color) color str LOG_NONE

typedef enum
{
    INFO = 333,
    WARN,
    FATA
} log_level;

#define __LOG_FILE(level, format, ...)                                                                            \
    do                                                                                                            \
    {                                                                                                             \
        extern char *check_level(log_level);                                                                      \
        char        *color = check_level(level);                                                                  \
        extern void  gen_logfile();                                                                               \
        gen_logfile();                                                                                            \
        extern FILE *log_ptr;                                                                                     \
        fprintf(log_ptr, "%s[%s]" LOG_NONE, color, #level);                                                       \
        fprintf(log_ptr, LOG_CYAN "%s:%d:%s " LOG_NONE format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        fflush(log_ptr);                                                                                          \
    }                                                                                                             \
    while (0)

// test macro: https://stackoverflow.com/questions/26099745/test-if-preprocessor-symbol-is-defined-inside-macro
/**
 * @brief Only Work for Boolean Macro with status: `undefine`, `0`, `>=1`(Need Add more macro placeholders)
 * IF We have `#define CONFIG_TEST 1`, then use the `IS_DEFINED(CONFIG_TEST)` Will get Macro `__PLACE_HOLDER_0` or
 * `__PLACE_HOLDER_1`. So 2 list (0,1,0) for defined, and (... 1,0) for not defined macro.
 * then choose second arg as return value.
 *
 */
#define __MACRO_STR(item)                     #item
#define __PLACE_HOLDER_1                      0,
#define __TAKE_SECOND_ARG(_ignored, arg, ...) arg

#define __MACRO_RESULT(_val)                  __TAKE_SECOND_ARG(_val 1, 0)
#define __MACRO_PROPERTY(_val)                __MACRO_RESULT(__PLACE_HOLDER_##_val)
#define ISDEF(macro)                          __MACRO_PROPERTY(macro)
// it ONLY works inside a function, since it calls `strcmp()`
// macros defined to themselves (#define A A) will get wrong results
// ""#macro expand macro name as a string, whereas __MACRO_STR(macro) expand the macro first
#define isdef(macro)                          (strcmp("" #macro, ""__MACRO_STR(macro)) != 0)

#define Error(format, ...)                           \
    do                                               \
    {                                                \
        if (ISDEF(GEN_LOGFILE))                      \
        {                                            \
            __LOG_FILE(FATA, format, ##__VA_ARGS__); \
        }                                            \
        printf(LOG_RED format "\n", ##__VA_ARGS__);  \
        assert(0);                                   \
    }                                                \
    while (0)

#define Assert(cond, format, ...)                        \
    do                                                   \
    {                                                    \
        if (!cond)                                       \
        {                                                \
            if (ISDEF(GEN_LOGFILE))                      \
            {                                            \
                __LOG_FILE(FATA, format, ##__VA_ARGS__); \
            }                                            \
            printf(LOG_RED format "\n", ##__VA_ARGS__);  \
            assert(cond);                                \
        }                                                \
    }                                                    \
    while (0)

// #####################################################################################
//  Geneal Code Macro
// #####################################################################################
#define Arrlen(arr)        (sizeof((arr)) / sizeof(arr[0]))
#define Panic(format, ...) Assert(0, format, ##__VA_ARGS__)
#define TODO               Panic("YOU Have to Implemented Here!!");
#define Checkret(action, resval, retval, msg, ...)               \
    do                                                           \
    {                                                            \
        if ((action) == resval)                                  \
        {                                                        \
            if (ISDEF(GEN_LOGFILE))                              \
            {                                                    \
                __LOG_FILE(INFO, msg, ##__VA_ARGS__);            \
            }                                                    \
            printf(LOG_PURPLE msg "\n" LOG_NONE, ##__VA_ARGS__); \
            return retval;                                       \
        }                                                        \
    }                                                            \
    while (0)
#define Checkerr(action, rtval, msg, ...) \
    do                                    \
    {                                     \
        if ((action) == rtval)            \
        {                                 \
            Error(msg, ##__VA_ARGS__);    \
        }                                 \
    }                                     \
    while (0)

#endif