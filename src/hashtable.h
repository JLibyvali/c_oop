#ifndef C_OOP_HASHTABLE__H
#define C_OOP_HASHTABLE__H

#include "debug.h"
#include "macro.h"
#include <stdatomic.h>

/**
 * @brief Using the HashTable to Emulate `Virtual Table`
 */

typedef struct
{
    unsigned int m_key;
    polyfn_t     m_func;
} hashitem;

typedef struct
{
    hashitem   **m_items;
    unsigned int m_capacity;
    unsigned int m_size;

} VTable;

// an array to store every declared Class virtual table.
extern VTable *vtable_array[];
extern atomic_uint _vtable_index;

unsigned int hash_pointer(void  **_obj);
VTable             *create_vtable();
void                delete_vtable(unsigned int _table_idx);
void                vtable_insert(unsigned int _table_idx, unsigned int _keyindex, polyfn_t _val);

// Help for generate general virtual function
#define __POLYFUNCTION_DECLARE(typename, name) \
    int name(typename *_objptr)               \
    {                                          \
    /* Do input type check */           \
    typename## _typecheck(*_objptr);        \
    unsigned int table_idx = (atomic_load(&_vtable_index)-1 );      \
    unsigned int key_idx = hash_pointer((void**)&_objptr->name);        \
    extern polyfn_t  __vtable_search(unsigned int _table_idx, unsigned int _key_idx);       \
    polyfn_t fn_p = __vtable_search(table_idx,key_idx);     \
    if(fn_p  == NULL) {         \
        __CALL_vtable_insert(table_idx,_objptr,name);     \
    }       \
    fn_p = __vtable_search(table_idx,key_idx);      \
    Checkerr(fn_p,NULL,"Insert functon "__MACRO_STR(name)" to Vtable FAILED!!");        \
    int res = fn_p(_objptr);       \
    return res;                            \
    }

#define __POLYFUNCTION_NEXT(typename, name, ...) \
    __POLYFUNCTION_DECLARE(typename, name)       \
    __VA_OPT__(__POLYFUNCTION_RECURSIVE(typename, __VA_ARGS__))

#define __POLYFUNCTION_RECURSIVE(typename, name, ...) \
    __POLYFUNCTION_DECLARE(typename, name)            \
    __VA_OPT__(__POLYFUNCTION_NEXT(typename, __VA_ARGS__))

#define POLYFUNCTION_DECLARE(typename, ...) __VA_OPT__(__POLYFUNCTION_RECURSIVE(typename, __VA_ARGS__))

// Help for generate Vtable
#define __VTABLE_GEN(_objptr, ...)                                                                  \
    do                                                                                              \
    {                                                                                               \
        /* IF can access this macro, meaning that the Declaration of Class must have polyfn name */ \
        /* So need create a  virtuable For  every polyfn name */                                    \
        vtable_array[atomic_load(&_vtable_index)] = create_vtable();                                \
        atomic_fetch_add(&_vtable_index, 1);                                                        \
    }                                                                                               \
    while (0)
#define __NEED_VTABLE(_objptr, ...) __VA_OPT__(__VTABLE_GEN(_objptr, __VA_ARGS__))
#endif