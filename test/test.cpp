#include <iostream>
#include <list>

#include <time.h>

extern "C"
{
#include "../src/array/CTL_array.h"
#include "../src/array/CTL_array.c"
#include "../src/algorithm/CTL_copy.h"
}

int main(void)
{
    CTL_array a, b;
    int array1[20] = {0,1,2,3,4,5,6,7,8,9};
    int array2[10] = {9,8,7,6,5,4,3,2,1,0};
    CTL_array_new(&a,array1, array1 + 10);
    CTL_array_new(&b,array2, array2 + 10);
    std::list<int> li;
    auto it = CTL_array_iterator_move(&b.begin, 2, false);
    CTL_rcopy(CTL_array, CTL_array, a.begin, a.end, it);
    std::cout<<array2<<std::endl<<b.begin.data<<std::endl;
    for (size_t i = 0; i < 10; i++)
    {
        std::cout<<array2[i] <<std::endl;
    }
    
    return 0;
}