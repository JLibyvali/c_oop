#include "hashtable.h"

#include "debug.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// ################################################################################################################
//  Hash Table related about function
// ###############################################################################################################
#define _TABLE_SIZE(size)                                                      \
    unsigned int   vtable_size        = (#size[0] == '\0') ? 256 : (size + 0); \
    VTable *vtable_array[size] = {};

#ifdef VTABLE_SIZE
_TABLE_SIZE(VTABLE_SIZE);
#else
_TABLE_SIZE();
#endif
// Indicate the nums of created vtable
atomic_uint   _vtable_index;

 unsigned int hash_pointer(void  **_obj)
{
    unsigned int res = 5381;

    void        *temp;
    while ((temp = *_obj++))
    {
        res = res * 33 + (uintptr_t)(temp);
    }

    return (res % vtable_size - 1);
}

static hashitem *create_item(unsigned int _key, polyfn_t _val)
{
    hashitem *item = malloc(sizeof(hashitem));
    Checkerr(item, NULL, "Malloc of HashTable Item FAILED!!");

    item->m_key  = _key;
    item->m_func = _val;

    return item;
}

VTable *create_vtable()
{
    // malloc space for vtable
    VTable *table = malloc(sizeof(VTable));
    Checkerr(table, NULL, "Malloc VTable FAILED!!");

    table->m_capacity = vtable_size;
    table->m_size     = 0;
    table->m_items    = malloc(vtable_size * sizeof(hashitem *));
    Checkerr(table->m_items, NULL, "Malloc For Vtable Items FAILED!!");

    // init hashtable item start value
    for (int i = 0; i < table->m_capacity; i++)
        table->m_items[i] = NULL;

    return table;
}

void delete_vtable(unsigned int _table_idx )
{
    VTable * _table_p = vtable_array[_table_idx];
    Checkerr(_table_p, NULL, "Delete vtable parameter is NULL");

    for (int i = 0; i < _table_p->m_capacity; i++)
        free(_table_p->m_items[i]);

    free(_table_p->m_items);
    free(_table_p);
}

void vtable_insert(unsigned int _table_idx, unsigned int _index, polyfn_t _val)
{
    Checkerr(_val, NULL, "Input `polyfn_t _val` parameter is NULL");

    VTable *_table = vtable_array[_table_idx];
    hashitem *item          = create_item(_index, _val);
    _table->m_items[_index] = item;

    if (_table->m_size < _table->m_capacity)
        _table->m_size++;
    else
        _table->m_size = _table->m_capacity;
}

polyfn_t __vtable_search(unsigned int _table_idx, unsigned int _key)
 {
     VTable *table = vtable_array[_table_idx];
     hashitem *item  = table->m_items[_key];

     if(item == NULL)
            return  NULL;
     else
            return item->m_func;

 }