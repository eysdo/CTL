#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <malloc.h>
#include <deque>
#include <array>

using namespace std;

int main()
{

    vector<int> d1;
    d1.push_back(1);
    d1.push_back(2);
    d1.pop_back();
    d1.insert(d1.end(),3);

    cout <<d1[2]<<endl;
    return 0;
    auto i = d1.end();
    //cout << *i._M_cur << endl;
    return 0;
}