//
// Created by jlibyvali on 1/3/25.
//
#include "c_oop.h"

Class(Tree, MEMBER(int m_id), );

int main()
{
    Tree *t  = new (Tree, 10);
    Tree *t2 = copy(Tree, t);

    delete (t);
    return 0;
}