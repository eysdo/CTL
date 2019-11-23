#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <malloc.h>
#include <deque>

using namespace std;

int main()
{
    deque<int> d1;

    int dim[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    d1.push_back(88);
    auto i = d1.begin();

    cout << *i._M_cur << endl;
    return 0;
}