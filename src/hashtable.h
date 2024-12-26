#ifndef C_OOP_HASHTABLE__H
#define C_OOP_HASHTABLE__H

#include "constant.h"
#include "debug.h"

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

static VTable  *create_vtable();
static void     delete_vtable(VTable *_table_p);
static void     vtable_insert(const void *_obj_tokey, polyfn_t _val);
static polyfn_t vtable_search(const void *_obj_tokey);

#endif