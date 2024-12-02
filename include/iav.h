#ifndef IAV_H
#define IAV_H

#include "config.h"

class IAV{

    public:
        IAV();
        ~IAV();
        
        void start();

    private:

        Config& cfg = Config::getInstance();
};


#endif