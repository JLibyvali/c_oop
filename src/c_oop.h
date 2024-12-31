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

#endif

#endif