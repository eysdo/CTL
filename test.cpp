#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <malloc.h>

using namespace std;

int main()
{
    int dim[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<int> d1;
    d1.push_back(88);
    auto i = d1.begin();

    cout << d1.size() << endl;
    return 0;
}