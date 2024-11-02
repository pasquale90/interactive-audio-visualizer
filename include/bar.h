#ifndef BAR_H
#define BAR_H

#include <vector>

class Bar{
    public:
        Bar ();
        Bar(int,int);
        int sum();
        int mul();

        std::vector<int> getData();
    private:
        std::vector<int> data;
};

#endif