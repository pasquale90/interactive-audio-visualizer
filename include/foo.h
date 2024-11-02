#ifndef FOO_H
#define FOO_H

#include "bar.h"

class Foo{
    public:
        Foo ();
        Foo(int,int);
        int sum();
        int subtract();
        int div();
        int mul();

        int checkBar(Bar&);
    private:
        int a,b;
};

#endif