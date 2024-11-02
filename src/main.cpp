#include "foo.h"
#include "bar.h"
#include <iostream>

int main(){
    int n1{3},n2{5};
    Foo f (n1,n2);
    Bar b (n1,n2);
    
    std::cout<<"Nums are "<<n1<<","<<n2<<std::endl;
    std::cout<<"Foo :"<<std::endl;
    printf("%d+%d=%d\n",n1,n2,f.sum());
    printf("%d-%d=%d\n",n1,n2,f.subtract());
    printf("%d/%d=%d\n",n1,n2,f.div());
    printf("%d*%d=%d\n",n1,n2,f.mul());
    printf("check bar =%d\n",f.checkBar(b));

    std::cout<<"Bar :"<<std::endl;
    printf("%d*%d=%d\n",n1,n2,b.sum());
    printf("%d^%d=%d\n",n1,n2,b.mul());
    return 0;
}