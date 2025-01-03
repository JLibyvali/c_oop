//
// Created by jlibyvali on 12/30/24.
//
#include "c_oop.h"
#include "debug.h"

Class(Tree, MEMBER(long m_high), METHOD(long (*get_high)(struct Tree *_in)), );

typedef struct
{
    int m_id;
} private;

long get_high(Tree *_in)
{
    Checkerr(_in, NULL, "Parameter is NULL.");
    return _in->m_high;
}

void *new_Tree_private()
{
    private *p = malloc(sizeof(private));
    *p         = (private){10};

    return (void *)p;
}

void delete_Tree_private(void *_in)
{
    Checkerr(_in, NULL, "Parameter is NULL.");
    free(_in);
}

void print(Tree *_in)
{
    const private *p = _in->m_private;
    printf("Datamemrbers public: %ld, private: %d .\n", _in->get_high(_in), p->m_id);
    return;
}

int main()
{
    printf("######################### Test Private data members #######################################\n");
    Tree *tree = new (Tree, 21341341, get_high, new_Tree_private());
    print(tree);
    delete (tree);
    return 0;
}
