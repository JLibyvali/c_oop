#ifndef C_OOP_CONSTANT__H
#define C_OOP_CONSTANT__H
#include <stddef.h>

// ########################################################################
//  Helper Macro function
// ######################################################################

typedef int (*polyfn_t)(void *);

// Help for generate declaration of  virtual function.
#define __POLYFN_DECL(typename, name) int (*name)(typename * _in);
#define __POLYFN_RECURSIVE(typename, name, ...) \
    __POLYFN_DECL(typename, name)               \
    __VA_OPT__(__POLYFN_NEXT(typename, __VA_ARGS__))

#define __POLYFN_NEXT(typename, name, ...) \
    __POLYFN_DECL(typename, name)          \
    __VA_OPT__(__POLYFN_RECURSIVE(typename, __VA_ARGS__))

#define POLYFN(typename, ...)        __VA_OPT__(__POLYFN_RECURSIVE(typename, __VA_ARGS__))

// Help for invoke vtable_insert()
#define __CALL_toparam(text1, text2) (#text1 "->" #text2)
#define __CALL_vtable_insert(_objptr, name) \
    vtable_insert(hash_pointer((void **)__CALL_toparam(_obj, name)), __CALL_toparam(_objptr, name));

#define __POLYFNNAME_NEXT(_objptr, name, ...) \
    __CALL_vtable_insert(_objptr, #name) __VA_OPT__(__POLYFNNAME_RECURSIVE(_objptr, __VA_ARGS__))

#define __POLYFNNAME_RECURSIVE(_objptr, name, ...) \
    __CALL_vtable_insert(_objptr, #name) __VA_OPT__(__POLYFNNAME_NEXT(_objptr, __VA_ARGS__))

#define CALL_vtable_insert(_objptr, ...) __POLYFNNAME_RECURSIVE(_objptr, __VA_ARGS__)

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