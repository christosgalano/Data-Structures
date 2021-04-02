#include "Map.hpp"

#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {

    srand(time(NULL));

    Map<int, double> map;

    map[1] = 110.5;

    cout << map[1] << ' ' << map[0] << '\n';
    cout << map.size() << '\n';

    map.remove(0);

    cout << map.size() << '\n';

    Map<string, string> mp;
    
    mp["Alex"] = "Bold";
    mp["Chris"];
    
    cout << mp.size() << '\n';

    mp.remove("Alex");
    cout << mp.size() << '\n';
}