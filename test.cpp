#include <iostream>
#include <vector>
#include <deque>

using namespace std;

struct test
{
    int a;
    int* b = &a;
};

int main(void)
{
    vector<test> t(1);
    cout << t.capacity()<<endl;
    test a;
    t.push_back(a);
    cout << t[0].b << endl;
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    t.push_back(a);
    cout << t[0].b << endl;
    return 0;
}