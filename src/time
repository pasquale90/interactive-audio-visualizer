#include <iostream>
#include <thread>
#include "timer.h"

/*
void def(char input){
    std::cout<<"Press \'P\' to pause the Task\'s timer and \'U\' to continue"<<std::endl;
    while(std::cin>>input){
//            pot->pause();
        }
        if (input=='U'){
            std::thread t3(&Potato::resume,pot);
            //potato->resume();
            if(t3.joinable()){
                t3.join();
                break;
            }
        }
    }

}

void def2(Potato *pot){
    //potato->start(3);
    if (pot->isdone()){
        std::cout<<"The process is done"<<std::endl;
    }
}
*/

int main()
{
    Timer *timer = new Timer(3);
    timer->inverse_count();
    if(timer->elapsed()){
        std::cout<<"Program terminated"<<std::endl;
    }

    /*
    std::thread t1(&Potato::start, potato, 5);
    char input;
    std::thread t2(def, input, potato);
    if(t1.joinable()){
        t1.join();
    }

    if(t2.joinable()){
        t2.join();
    }
    std::thread t3(def2,potato);
    if(t3.joinable()){
        t3.join();
    }*/

    return 0;
}
