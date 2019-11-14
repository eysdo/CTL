#include <vector>
#include <iostream>
#include <deque>
int main(void)
{
    std::deque<int> de;
    int t;
    de.pop_back(t)
    std::vector<int> vec;

vec.push_back(6);
vec.push_back(-17);
vec.push_back(12);
vec.push_back(13);
vec.push_back(14);
// Deletes the second element (vec[1])
vec.erase(vec.begin() + 1);
std::cout<<vec[2]<<std::endl;
vec.erase(vec.begin() + 2);
std::cout<<vec[2]<<std::endl;
    return 0;
}