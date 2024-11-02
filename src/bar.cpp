#include "bar.h"
#include <cmath>

Bar::Bar():data(10,0) {}

Bar::Bar(int size,int val) : data(size,val) {}

int Bar::sum(){
    int s {0};
    for (auto v:data){
        s += v;
    }
    return s;
}

int Bar::mul(){
    return std::pow(data[0],data.size());
}

std::vector<int> Bar::getData(){
    return data;
}