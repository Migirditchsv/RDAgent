// sandbox

#include <iostream>
#include <vector>
#include <algorithm>

#include "AgentInterface.h"

int main()
{
    using namespace std;

    vector<int> x(13);
    std::iota(x.begin(), x.end(),0);

    cout<<"{"; 
    for(std::vector<int>::const_iterator i=x.begin(); i != x.end(); i++)
    {
        cout<<*i<<",";
    }
    cout<<"}";
    return(0);
}

