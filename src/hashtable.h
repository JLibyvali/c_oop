#ifndef C_OOP_HASHTABLE__H
#define C_OOP_HASHTABLE__H

#include "debug.h"
#include "macro.h"

#include <stdatomic.h>

/**
 * @brief A type decalration of `virtual method` and `virtual function`. Case When at compile time, code cannot get type
 * of declared class.
 * @param void* For class object  pointer.
 * @param void* For another given parameters.
 */
typedef int (*polyfn_t)(void *, void *);

/**
 * @brief The hashtable item struct, `m_key` was calculated hash value, `m_func` was mapped function pointer.
 */
typedef struct
{
    unsigned int m_key;
    polyfn_t     m_func;
} hashitem;

/**
 * @brief THe Hashtable struct, `m_items` is the hashtable, very fast to search.
 * @param m_capacity The CPP container capacity concept, is the allocated memory size, default capacity is 256. You can
 * changed it by define macro.
 * @param m_size The CPP container size concept, is the elements index
 */
typedef struct
{
    hashitem   **m_items;
    unsigned int m_capacity;
    unsigned int m_size;

} VTable;

/**
 * @brief The Virtaul table array to store every named virtual method.
 */
extern VTable     *vtable_array[];
/**
 * @brief The atomi virtual table index.
 */
extern atomic_uint _vtable_index;

/**
 * @brief function to calculate hash value, by object method address. IF NULL will compiled error.
 * @param _obj_method the object method address, is hetergenous in every class.
 * @return unsigned int hashvalue.
 */
unsigned int       hash_pointer(void **_obj_method);
/**
 * @brief Create a virtual table for every named virtual mehotd.
 * @return virtual table pointer. Contained malloc() error checked, so it's sagfe to use.
 */
VTable            *create_vtable();
/**
 * @brief  Free() a table completely, Contained parameter error check.
 * @param _table_idx the virtual table deleted idnex.
 */
void               delete_vtable(unsigned int _table_idx);
/**
 * @brief Insert a table item into vritual table.
 * @param _table_idx The index of your insert  target virtual table.
 * @param _keyindex The hashvalue to be the key.
 * @param _val The polyfn_t functon pointer to be the value.
 */
void               vtable_insert(unsigned int _table_idx, unsigned int _keyindex, polyfn_t _val);

// ######################################################################################################################
// # Macro definition
// ######################################################################################################################
/**
 * @brief WHY: The virtual function definition, contains the typecheck, and return value.
 *        HOW: First. do typecheck, if not correct type, will occurr a compiler error.
 *             Second. Search current object pointer's method which name is same as callee function. e.g. if you invoke
 *                     `print(object)`, it will search `object->print` method in virtual table.
 *                      So if you givent a wrong name, will occur compiled error case none that named `member method` in
 * struct declaration.
 *             Third. If search return NULL , do once insert action. Then, If still search NULL, meanning that insert
 * action failed.
 * @param typename The parameter pointer type, should be a name of declared class.
 * @param name  function name, keep same with declared `virtual method` in class.
 */
#define __POLYFUNCTION_DECLARE(typename, name)                                                       \
    int name(typename *_objptr, void *param)                                                         \
    {                                                                                                \
        /*Call type check funciton, The compiler should keep flag `-Wincompatiable-pointer-types` */ \
        typename##_method_typecheck(_objptr->name);                                                  \
        unsigned int    table_idx = (atomic_load(&_vtable_index) - 1);                               \
        unsigned int    key_idx   = hash_pointer((void **)&_objptr->name);                           \
        extern polyfn_t __vtable_search(unsigned int _table_idx, unsigned int _key_idx);             \
        polyfn_t        fn_p = __vtable_search(table_idx, key_idx);                                  \
        if (fn_p == NULL)                                                                            \
        {                                                                                            \
            __CALL_vtable_insert(table_idx, _objptr, name);                                          \
        }                                                                                            \
        fn_p = __vtable_search(table_idx, key_idx);                                                  \
        Checkerr(fn_p, NULL, "Insert functon "__MACRO_STR(name) " to Vtable FAILED!!");              \
        int res = fn_p(_objptr, param);                                                              \
        return res;                                                                                  \
    }

#define __VTABLE_GEN(_objptr, ...)                                                              \
    /* IF can access this macro, meaning that the Declaration of Class must have polyfn name */ \
    /* So need create a  virtuable For  every polyfn name */                                    \
    unsigned int index  = atomic_load(&_vtable_index);                                          \
    vtable_array[index] = create_vtable();                                                      \
    atomic_fetch_add(&_vtable_index, 1);

/**
 * @brief WHY: To evaluate is there a need to create virtual table, if there exist a named virtual function.
 *        HOW: using the `__VA_OPT__` macro feature, if givend named, that will expand and parse it.
 * @param _objptr object pointer, generate by malloc().
 */
#define __NEED_VTABLE(_objptr, ...) __VA_OPT__(__VTABLE_GEN(_objptr, __VA_ARGS__))

#endif