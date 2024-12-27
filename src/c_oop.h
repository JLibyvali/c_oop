#ifndef C_OOP__H
#define C_OOP__H

#include "debug.h"
#include "hashtable.h"
#include "macro.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#define MEMBER(...) __VA_OPT__(__VA_ARGS__)
#define METHOD(...) __VA_OPT__(__VA_ARGS__)

#define Class(_typename, _datamember, _classmethod, ...)                             \
    typedef struct _typename                                                         \
    {                                                                                \
        _datamember;                                                                 \
        _classmethod;                                                                \
        void *m_private;                                                             \
        POLYFN(struct _typename, __VA_ARGS__)                                        \
    } _typename;                                                                     \
    static inline _typename *new_##_typename()                                       \
    {                                                                                \
        _typename *obj = (_typename *)malloc(sizeof(_typename));                     \
        Checkerr(obj, NULL, "Malloc of type: "__MACRO_STR(_typename) "FAILED!!");    \
        /* Check If class has virtual function to generate a vtable or not*/         \
        __NEED_VTABLE(obj, __VA_ARGS__);                                  \
        memset(obj, 0, sizeof(_typename));                                           \
        return obj;                                                                  \
    }                                                                                \
    static inline void delete_##_typename(_typename *obj)                            \
    {                                                                                \
        Checkerr(obj, NULL, __MACRO_STR(delete_##_typename) " parameter is NULL!!"); \
        free(obj);                                                                   \
    }

#define new(_class, ...)                           \
    ({                                             \
        _class *temp = new_##_class();             \
        __VA_OPT__(*temp = (_class){__VA_ARGS__};) \
        temp;                                      \
    })

#define delete(_type_ptr, ...)                   \
    ({                                           \
        if (_type_ptr)                           \
        {                                        \
            free(_type_ptr);                     \
            __VA_OPT__(__Free_list(__VA_ARGS__)) \
        }                                        \
    })

#endif