#include "Array.h"
#include <iostream>
#include <vector>

using namespace std;


int main()
{
    Array<int> a;
    for (int i = 0; i < 10; ++i) {
        a.insert(i + 1);
    } 

    for (auto it = a.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << " ";
    std::cout << std::endl;

    for (int i = 0; i < a.size(); ++i)
        a[i] *= 2;

    for (auto it = a.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << " ";
    std::cout << std::endl;

}