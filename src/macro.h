#ifndef C_OOP_MACRO__H
#define C_OOP_MACRO__H

// ######################################################################################################################
// # Helper Macro definiction
// ######################################################################################################################
/**
 * @brief The help macors for `Class` macro
 */
#define MEMBER(...)                       __VA_OPT__(__VA_ARGS__)
#define METHOD(...)                       __VA_OPT__(__VA_ARGS__)

/**
 * @brief Helper macro for Polymorphism method declaration in Class.
 */
#define __POLYMETHOD_DECL(typename, name) int (*name)(typename * _objptr, void *param);
#define __POLYMETHOD_RECURSIVE(typename, name, ...) \
    __POLYMETHOD_DECL(typename, name)               \
    __VA_OPT__(__POLYMETHOD_NEXT(typename, __VA_ARGS__))
#define __POLYMETHOD_NEXT(typename, name, ...) \
    __POLYMETHOD_DECL(typename, name)          \
    __VA_OPT__(__POLYMETHOD_RECURSIVE(typename, __VA_ARGS__))
/**
 * @brief WHY: To delcare various `virtual method` in class just write once.
 *        HOW: Use the macro recursive to extrace every name from __VA_ARGS__.
 * @param typename  The declaration class name. Only When given `virtual method` name will do work later.
 */
#define POLYMETHOD_DECLARE(typename, ...) __VA_OPT__(__POLYMETHOD_RECURSIVE(typename, __VA_ARGS__))

/**
 * @brief Helper macro for polymorphism functon declaration at the end of Class.
 */
#define __POLYFUNCTION_NEXT(typename, name, ...) \
    __POLYFUNCTION_DECLARE(typename, name)       \
    __VA_OPT__(__POLYFUNCTION_RECURSIVE(typename, __VA_ARGS__))
#define __POLYFUNCTION_RECURSIVE(typename, name, ...) \
    __POLYFUNCTION_DECLARE(typename, name)            \
    __VA_OPT__(__POLYFUNCTION_NEXT(typename, __VA_ARGS__))
/**
 * @brief WHY: Acompany with declaration of `virtual method`, declare this function to invoke correct type's virtual
 * method. HOW: declare a function, and check input param type.
 * @param typename The Class declaration name.
 */
#define POLYFUNCTION_DECLARE(typename, ...) __VA_OPT__(__POLYFUNCTION_RECURSIVE(typename, __VA_ARGS__))

/**
 * @brief WHY: convert macro parameters to pertain the `vtable_insert()` function.
 *        HOW: Use macro text replace.
 * @param _tableidx Virtaul table index in `_vtable_array[]` array. To find correct table to insert.
 * @param _objptr  The object pointer which return by type inner new function.
 * @param name The virtual method name you wanna insert to virtual table.
 */
#define __CALL_vtable_insert(_tableidx, _objptr, name)         \
    unsigned int _key = hash_pointer((void **)&_objptr->name); \
    vtable_insert(_tableidx, _key, (polyfn_t)(_objptr->name));
#define __CALL_vi_NEXT(_tableidx, _objptr, name, ...) \
    __CALL_vtable_insert(_tableidx, _objptr, name) __VA_OPT__(__CALL_vi_RECURSIVE(_tableidx, _objptr, __VA_ARGS__))
#define __CALL_vi_RECURSIVE(_tableidx, _objptr, name, ...) \
    __CALL_vtable_insert(_tableidx, _objptr, name) __VA_OPT__(__CALL_vi_NEXT(_tableidx, _objptr, __VA_ARGS__))
/**
 * @brief WHY: To insert varitous `function pointer` into created `virtual table` When using `new()` macro to instance a
 * object. User should not call the macro, if passing a NULL name or error parameters, may occurr undefined behaviour
 * and very hard to debug.
 * @param _taleidx The virtual table index of the `_vtable_array[]`, bacause every declaration name of virtual method
 * will generate a virtual table.
 * @param _objptr The object pointer which malloced by inner type new funcion in heap.
 */
#define CALL_vtable_insert(_taleidx, _objptr, ...) __CALL_vi_RECURSIVE(_taleidx, _objptr, __VA_ARGS__)

/**
 * @brief A macro used to `vectorize` C free() function, then you can use it as `delete(obj1,obj2,obj3,...)`
 */
#define __Free_normals(...)                                    \
    {                                                          \
        void  *stopper     = (int[]){0};                       \
        void **list_tofree = (void *[]){__VA_ARGS__, stopper}; \
        for (int i = 0; list_tofree[i] != stopper; i++)        \
            free(list_tofree[i]);

// ########################################################################
//  Register OOP type information
// ######################################################################

#endif