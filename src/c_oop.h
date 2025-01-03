#ifndef C_OOP__H
#define C_OOP__H

#include "debug.h"
#include "hashtable.h"
#include "macro.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#ifdef NEED_PRIVATE
/**
 * @brief  This class defined a private pointer;
 * @brief  WHY: To  emulate CPP `class` keyword, will generate `new/delete/copy/move` inner function for every
 * declaretion of class. HOW: Just use some macro magic.
 * @param _typename You wanna declare class name, like CPP `class name{};` .
 * @param _datamember The class's data members, You can use a help macro `MEMBER` to more clear.
 * @param _classmethod The class's method, You can use a help macr `METHOD` to more clear.
 */
#    define Class(_typename, _datamember, _classmethod, ...)                                             \
        /**                                                                                              \
         * @brief the fundmental data struct to implemet `class` keyword                                 \
         * @param m_private the pointer to private struct type to implement `public` and `private` scope \
         * @param POLYMETHOD_DECLARE helper macro to delcare virtual method, just do declare work.       \
         */                                                                                              \
        typedef struct _typename                                                                         \
        {                                                                                                \
            _datamember;                                                                                 \
            _classmethod;                                                                                \
            void *m_private;                                                                             \
            POLYMETHOD_DECLARE(struct _typename, __VA_ARGS__)                                            \
        } _typename;                                                                                     \
        /* Declare a type validator function to check input paramter type at compile time. */            \
        void                     _typename##_method_typecheck(int (*)(_typename *, void *)) {}           \
        /* Declare a private new/delete function. */                                                     \
        extern void             *new_##_typename##_private();                                            \
        void                     delete_##_typename##_private(void *_in);                                \
        /* Declared Type inner new function, will called by new macro.*/                                 \
        static inline _typename *new_##_typename()                                                       \
        {                                                                                                \
            _typename *obj = (_typename *)malloc(sizeof(_typename));                                     \
            Checkerr(obj, NULL, "Malloc of type: "__MACRO_STR(_typename) "FAILED!!");                    \
            /* Check If class has virtual method to generate a virtual table */                          \
            __NEED_VTABLE(obj, __VA_ARGS__);                                                             \
            memset(obj, 0, sizeof(_typename));                                                           \
            return obj;                                                                                  \
        }                                                                                                \
        /* Declared Type inner delete function, will called by delete macro.*/                           \
        static inline void delete_##_typename(_typename *obj)                                            \
        {                                                                                                \
            Checkerr(obj, NULL, __MACRO_STR(delete_##_typename) " parameter is NULL!!");                 \
            free(obj);                                                                                   \
        }                                                                                                \
        POLYFUNCTION_DECLARE(_typename, __VA_ARGS__)

/**
 * @brief WHY: To emulate CPP `new` operator, not atomic options, contain failed malloc check.
 * @param _class The declared class name
 * @param Another params given NULL if not used.
 */
#    define new(_class, ...)                           \
        ({                                             \
            _class *temp = new_##_class();             \
            __VA_OPT__(*temp = (_class){__VA_ARGS__};) \
            temp;                                      \
        })

/**
 * @brief WHY: To emulate CPP `delete` keyword, you can delete(obj1,obj2,obj3,...)
 * @param _type_ptr The object pointer you wanna deleted.
 */
#    define delete(_type_ptr, ...)                   \
        ({                                           \
            if (_type_ptr)                           \
            {                                        \
                free(_type_ptr);                     \
                __VA_OPT__(__Free_list(__VA_ARGS__)) \
            }                                        \
        })

#else
/**
 * @brief  This class do not defined a private pointer.
 * @brief  WHY: To  emulate CPP `class` keyword, will generate `new/delete/copy/move` inner function for every
 * declaretion of class. HOW: Just use some macro magic.
 * @param _typename You wanna declare class name, like CPP `class name{};`.
 * @param _datamember The class's data members, You can use a help macro `MEMBER` to more clear.
 * @param _classmethod The class's method, You can use a help macr `METHOD` to more clear.
 */
#    define Class(_typename, _datamember, _classmethod, ...)                                             \
        /**                                                                                              \
         * @brief the fundmental data struct to implemet `class` keyword                                 \
         * @param m_private the pointer to private struct type to implement `public` and `private` scope \
         * @param POLYMETHOD_DECLARE helper macro to delcare virtual method, just do declare work.       \
         */                                                                                              \
        typedef struct _typename                                                                         \
        {                                                                                                \
            _datamember;                                                                                 \
            _classmethod;                                                                                \
            POLYMETHOD_DECLARE(struct _typename, __VA_ARGS__)                                            \
            atomic_uint __ref;                                                                           \
        } _typename;                                                                                     \
        /* Declare a type validator function to check input paramter type at compile time. */            \
        void                     _typename##_method_typecheck(int (*)(_typename *, void *)) {}           \
        /* Declared Type inner new function, will called by new macro.*/                                 \
        static inline _typename *new_##_typename()                                                       \
        {                                                                                                \
            _typename *obj = (_typename *)malloc(sizeof(_typename));                                     \
            Checkerr(obj, NULL, "Malloc of type: "__MACRO_STR(_typename) "FAILED!!");                    \
            /* Check If class has virtual method to generate a virtual table */                          \
            __NEED_VTABLE(obj, __VA_ARGS__);                                                             \
            memset(obj, 0, sizeof(_typename));                                                           \
            return obj;                                                                                  \
        }                                                                                                \
        /* Declared Type inner delete function, will called by delete macro.*/                           \
        static inline void delete_##_typename(_typename *obj)                                            \
        {                                                                                                \
            Checkerr(obj, NULL, __MACRO_STR(delete_##_typename) " parameter is NULL!!");                 \
            free(obj);                                                                                   \
        }                                                                                                \
        POLYFUNCTION_DECLARE(_typename, __VA_ARGS__)

/**
 * @brief WHY: To emulate CPP `new` operator, not atomic options, contain failed malloc check.
 * @param _class The declared class name
 * @param Another params given NULL if not used.
 */
#    define new(_class, ...)                           \
        ({                                             \
            _class *temp = new_##_class();             \
            __VA_OPT__(*temp = (_class){__VA_ARGS__};) \
            atomic_init(&temp->__ref, 1);              \
            temp;                                      \
        })

/**
 * @brief WHY: To emulate CPP `delete` keyword, you can delete(obj1,obj2,obj3,...)
 * @param _type_ptr The object pointer you wanna deleted.
 */
#    define delete(_type_ptr, ...)                            \
        ({                                                    \
            if (_type_ptr)                                    \
            {                                                 \
                atomic_fetch_sub(&_type_ptr->__ref, 1);       \
                if (atomic_load(&_type_ptr->__ref) == 0)      \
                {                                             \
                    /*TODO: Register class meta information*/ \
                }                                             \
                free(_type_ptr);                              \
                __VA_OPT__(__Free_normals(__VA_ARGS__))       \
            }                                                 \
        })

/**
 * @brief WHY: To implement shallow copy, using reference count.
 * @param _class The object pointer class name.
 * @param _type_ptr The copied target object pointer.
 */
#    define copy(_class, _type_ptr)                                                                   \
        ({                                                                                            \
            Checkerr(_type_ptr, NULL, __MACRO_STR(copy) " macro function parameter is NULL input!!"); \
            _class *ptr = malloc(sizeof(_class));                                                     \
            Checkerr(ptr, NULL, "Malloc for "__MACRO_STR(copy) " function memory FAILED!!");          \
            *ptr = *_type_ptr;                                                                        \
            atomic_fetch_add(&ptr->__ref, 1);                                                         \
            ptr;                                                                                      \
        })

#endif

#endif