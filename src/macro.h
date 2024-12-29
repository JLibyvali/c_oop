#ifndef C_OOP_MACRO__H
#define C_OOP_MACRO__H
#include <stddef.h>

// ########################################################################
//  Helper Macro function
// ######################################################################

typedef int (*polyfn_t)(void *);

// Help for generate Declaration of virtual method
#define __POLYMETHOD_DECL(typename, name) int (*name)(typename * _objptr);
#define __POLYMETHOD_RECURSIVE(typename, name, ...) \
    __POLYMETHOD_DECL(typename, name)               \
    __VA_OPT__(__POLYMETHOD_NEXT(typename, __VA_ARGS__))

#define __POLYMETHOD_NEXT(typename, name, ...) \
    __POLYMETHOD_DECL(typename, name)          \
    __VA_OPT__(__POLYMETHOD_RECURSIVE(typename, __VA_ARGS__))

#define POLYMETHOD_DECLARE(typename, ...) __VA_OPT__(__POLYMETHOD_RECURSIVE(typename, __VA_ARGS__))

// Help for invoke vtable_insert()
// #define __CALL_toparam(_objptr, _method)        (_objptr##->##_method)

#define __CALL_vtable_insert(_tableidx, _objptr, name)         \
    unsigned int _key = hash_pointer((void **)&_objptr->name); \
    vtable_insert(_tableidx, _key, (polyfn_t)(_objptr->name));

#define __GETNAME_NEXT(_tableidx, _objptr, name, ...) \
    __CALL_vtable_insert(_tableidx, _objptr, name) __VA_OPT__(__GETNAME_RECURSIVE(_tableidx, _objptr, __VA_ARGS__))

#define __GETNAME_RECURSIVE(_tableidx, _objptr, name, ...) \
    __CALL_vtable_insert(_tableidx, _objptr, name) __VA_OPT__(__GETNAME_NEXT(_tableidx, _objptr, __VA_ARGS__))

#define CALL_vtable_insert(_taleidx, _objptr, ...) __GETNAME_RECURSIVE(_taleidx, _objptr, __VA_ARGS__)

// Help for free(...)
#define __Free_list(...)                                       \
    {                                                          \
        void  *stopper     = (int[]){0};                       \
        void **list_tofree = (void *[]){__VA_ARGS__, stopper}; \
        for (int i = 0; list_tofree[i] != stopper; i++)        \
            free(list_tofree[i]);                              \
    }

// ########################################################################
//  Register OOP type information
// ######################################################################

#endif