#ifndef C_OOP_HASHTABLE__H
#define C_OOP_HASHTABLE__H

#include "debug.h"
#include "macro.h"

/**
 * @brief Using the HashTable to Emulate `Virtual Table`
 *
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

extern unsigned int hash_pointer(const void **_obj);
void                create_vtable();
void                delete_vtable(VTable *_table_p);
void                vtable_insert(unsigned int _index, polyfn_t _val);

// Help for generate Vtable
#define __VTABLE_GEN(_objptr, ...)                                                                  \
    do                                                                                              \
    {                                                                                               \
        /* IF can access this macro, meaning that the declaration of class must contain polyfn_t */ \
        extern atomic_bool _need_vtable;                                                            \
        /* Ensure Singleton Vtable created*/                                                        \
        if (!atomic_load(&_need_vtable))                                                            \
        {                                                                                           \
            atomic_store(&_need_vtable, 1);                                                         \
            create_vtable();                                                                        \
            CALL_vtable_insert(_objptr, ...)                                                        \
        }                                                                                           \
        CALL_vtable_insert(_objptr, ...)                                                            \
    }                                                                                               \
    while (0)
#define __NEED_VTABLE(_objptr, ...) __VA_OPT__(__VTABLE_GEN(_objptr, __VA_ARGS__))
#endif