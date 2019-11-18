#include <vector>
#include <iostream>
#include <deque>
#include <map>
#include <list>
using namespace std;

int main(void)
{
    vector<int> l;
    l.push_back(1);
    auto i = l.end();
    while (1)
    {
        cout<<*(i += 1000)<<endl;
    }
    
    cout<<*(++i)<<endl;
}