#include <iostream>
#include <memory>
#include <vector>

const int set[10] { 1, 1, 1, 4, 4, 3, 3, 3, 3, 3 };

const std::vector<int> set_relatives()
{
    int n = 10;
    std::vector<int> r(n);



    for(int s = 0; s < n; ++s)
    {
        int pos = 0;
        int car = set[s];
        r[s] = pos;

        for(int i = s + 1; i < n; ++i)
        {
            if(car == set[i]) 
            {
                r[i] = ++pos;
                s++;
            }
        }
    }
    return r;
}



int main()
{
    auto v = set_relatives();
    for(int i = 0; i < v.size(); ++i)
    {
        std::cout<<v[i]<<" ";
    }
    return 0;
}