#include "Vector.hpp"

#include <iostream>

using namespace std;

int main() {
    Vector<int> vec;

    for (int i = 0; i < 5; ++i)
        vec.push_back(i);
    
    vec.print();

    cout << '\n' << vec[2] << '\n';

    for (auto i : vec)
        cout << i << ' ';

    cout << '\n';

    vec.clear();

    for (int i = 0; i < 100; ++i) {
        vec.push_back(i);
        cout << "size = " << vec.size() << "  ,  capacity = " << vec.capacity() << '\n';
    }
    

}