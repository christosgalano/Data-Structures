#include "Map.hpp"

#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {

    srand(time(NULL));

    Map<int, string> map;

    std::vector<int> vec;
    for (int i = 0; i < 65; ++i)
        vec.push_back(rand() % 100);

    for (int i = 0; i < 65; ++i)
        map.insert(vec[i], "hello");

    std::cout << map.size() << '\n';

    for (int i = 0; i < 65; ++i)
        std::cout << vec[i] << ' ' << map[vec[i]] << '\n';
}