#ifndef C_OOP_CONSTANT__H
#define C_OOP_CONSTANT__H

typedef struct
{
    int (*m_dispatchfn)(struct Base *_obj);
    void *m_private;
} Base;

typedef int (*polyfn_t)(Base *);

typedef struct
{
    struct Base;
} Derived;

#define C_OOP_INIT(_type)

#endif