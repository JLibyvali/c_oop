#include "c_oop.h"

#include "debug.h"

/**
 * @brief WHY: If user defined macro `NEED_PRIVATE`, will case the `void *m_private` in every class is valid, can valued
 * as NULL or a private struct pointer.
 * @param _class    The public class name
 * @param _datamember The private struct datamembers.
 * @param _method the private struct method.
 */
#ifdef NEED_PRIVATE
#    define Class_private(_typename, _datamember, _method)                                              \
        typedef struct                                                                                  \
        {                                                                                               \
            _datamember;                                                                                \
            _method;                                                                                    \
        } _typename##_priva;                                                                            \
        _typename##_priva *new_##_typename##_private()                                                  \
        {                                                                                               \
            _typename##_priva *temp = malloc(sizeof(_typename##_priva));                                \
            Checkerr(_typename##_priva, NULL, "Malloc() of"__MACRO_STR(_typename##_priva) " FAILED!!"); \
            return temp;                                                                                \
        }
#endif
