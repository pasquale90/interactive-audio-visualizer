#include "foo.h"

Foo::Foo(): a(0), b(0) {}

Foo::Foo(int n1, int n2): a(n1), b(n2) {}

int Foo::sum(){
    return a+b;
}

int Foo::subtract(){
    return a-b;
}

int Foo::div(){
    return a/b;
}

int Foo::mul(){
    return a*b;
}

int Foo::checkBar(Bar& bar){
    auto data = bar.getData();
    return !data.empty() ? data[0] : -1;
}