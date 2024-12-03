#include "iav.h"

#include <iostream>

IAV::IAV()
{
    cfg.display();
}

IAV::~IAV(){

}

void IAV::start(){
    std::cout<<"Hello IAV!"<<std::endl;
}