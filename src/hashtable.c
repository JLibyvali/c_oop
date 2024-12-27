#include "hashtable.h"

#include "debug.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// ################################################################################################################
//  Hash Table related about function
// ###############################################################################################################
#define _TABLE_SIZE(size)                                               \
    unsigned int   vtable_size = (#size[0] == '\0') ? 256 : (size + 0); \
    static VTable *vtable_ptr  = NULL

#ifdef VTABLE_SIZE
_TABLE_SIZE(VTABLE_SIZE);
#else
_TABLE_SIZE();
#endif

unsigned int hash_pointer(const void **_obj)
{
    unsigned int res = 5381;

    void        *temp;
    while ((temp = *_obj++))
    {
        res = res * 33 + (uintptr_t)(temp);
    }

    return (res % vtable_size - 1);
}

// indicate the  programme need a singleton vtable or note.
atomic_bool      _need_vtable;

static hashitem *create_item(unsigned int _key, polyfn_t _val)
{
    hashitem *item = malloc(sizeof(hashitem));
    Checkerr(item, NULL, "Malloc of HashTable Item FAILED!!");

    item->m_key  = _key;
    item->m_func = _val;

    return item;
}

void create_vtable()
{
    // malloc space for vtable
    vtable_ptr = malloc(sizeof(VTable));
    Checkerr(vtable_ptr, NULL, "Malloc VTable FAILED!!");

    vtable_ptr->m_capacity = vtable_size;
    vtable_ptr->m_size     = 0;
    vtable_ptr->m_items    = malloc(vtable_size * sizeof(hashitem *));

    // init hashtable item start value
    for (int i = 0; i < vtable_ptr->m_capacity; i++)
        vtable_ptr->m_items[i] = NULL;
    Checkerr(vtable_ptr->m_items, NULL, "Malloc For Vtable Items FAILED!!");

    return;
}

void delete_vtable(VTable *_table_p)
{
    Checkerr(_table_p, NULL, "Delete vtable parameter is NULL");

    for (int i = 0; i < _table_p->m_capacity; i++)
        free(_table_p->m_items[i]);

    free(_table_p->m_items);
    free(_table_p);
}

void vtable_insert(unsigned int _index, polyfn_t _val)
{
    Checkerr(_val, NULL, "Create HashTable Item parameter is NULL");

    hashitem *item              = create_item(_index, _val);
    vtable_ptr->m_items[_index] = item;

    if (vtable_ptr->m_size < vtable_ptr->m_capacity)
        vtable_ptr->m_size++;
    else
        vtable_ptr->m_size = vtable_ptr->m_capacity;
}