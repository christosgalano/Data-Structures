#include "Graph.hpp"

#include <string>
#include <iostream>

int main() {

    // Graph<int> g;

    // for (int i = 0; i < 5; ++i)
    //     g.add_vertex(i);

    // g.add_edge(0, 0);

    // g.add_edge(0, 1);
    // g.add_edge(0, 2);
    // g.add_edge(0, 3);

    // g.add_edge(1, 4);
    // g.add_edge(1, 2);

    // g.add_edge(3, 2);

    // std::cout << "\n\n";

    // g.print();

    // std::cout << "\n\n";

    // g.remove_edge(0, 0);

    // g.remove_edge(0, 1);

    // g.remove_edge(1, 4);

    // g.print();

    // std::cout << "\n\n";

    // g.remove_vertex(0);

    // g.print();


    // std::cout << "\n\n";


    // for (auto i : g.get_vertices())
    //     std::cout << i << ' ';

    // std::cout << "\n\n";

    Graph<std::string> fb;

    fb.add_vertex("John");
    fb.add_vertex("Chris");
    fb.add_vertex("Maria");
    fb.add_vertex("Allan");
    fb.add_vertex("Joy");


    fb.add_edge("John", "Chris");
    fb.add_edge("John", "Maria");
    fb.add_edge("John", "Joy");

    fb.add_edge("Allan", "Joy");

    fb.add_edge("Maria", "Joy");

    for (auto name : fb.get_vertices()) {
        std::cout << name << ": ";
        for (auto v : fb.get_neighboors(name))
            std::cout << v << ' ';
        std::cout << '\n';
    }


    std::cout << fb.get_no_vertices() << ' ' << fb.get_no_edges() << "\n\n";

    fb.add_vertex("Alex");

    fb.remove_vertex("John");

    fb.remove_edge("Maria", "Joy");

    std::cout << "\n\n";

    for (auto name : fb.get_vertices()) {
        std::cout << name << ": ";
        for (auto v : fb.get_neighboors(name))
            std::cout << v << ' ';
        std::cout << '\n';
    }

    std::cout << "\n\n";
    for (auto name : fb.get_vertices())
        std::cout << name << ' ';

    std::cout << "\n\n";
}
