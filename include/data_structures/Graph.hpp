#pragma once

#include "Map.hpp"

namespace data_structures {

template <typename T>
class Graph {
public:
    Graph() = default;
    Graph(const std::initializer_list<T>& list);

    void add_vertex(const T& value);
    void remove_vertex(const T& value);

    void add_edge(const T& v1, const T& v2, unsigned weight);
    void remove_edge(const T& v1, const T& v2);

    void change_weight(const T& v1, const T& v2, unsigned new_weight);
    unsigned get_weight(const T& v1, const T& v2);

    Vector<T> get_vertices();
    Vector<T> get_neighboors(const T& vertex);

    std::size_t get_no_vertices() const { return no_vertices; }
    std::size_t get_no_edges()    const { return no_edges;    }
    
private:
    class Vertex;
    class Edge;

    Map<T, Vertex> vertices;
    std::size_t no_vertices {};
    std::size_t no_edges    {};

    bool validate(const T& v1, const T& v2);
};


template<typename T>
Graph<T>::Graph(const std::initializer_list<T>& list) {
    for (auto& val : list)
        add_vertex(val);
}


template <typename T>
bool Graph<T>::validate(const T& v1, const T& v2) {
    return ( vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end() ) ? false : true;
}


template <typename T>
void Graph<T>::add_vertex(const T& value) {
    vertices[value] = Vertex{value};
    ++no_vertices;
}


template <typename T>
void Graph<T>::remove_vertex(const T& value) {
    auto iter = vertices.find(value);
    if (iter == vertices.end())
        throw std::invalid_argument("no such vertex in the graph");

    for (auto& neighboor : iter->second.get_neighboors()) {
        vertices[neighboor.get_dest().get_data()].remove_edge(iter->second);
        no_edges -= 2;
    }

    vertices.remove(value);
    --no_vertices;
}


/// NOTE : we do not check that v1 != v2. This implementation is based on the assumption ///
/// that the graph does not support duplicate vertices.                                  ///


// If an edge between these vertices already exists, its weight gets updated to the argument's new weight
template <typename T>
void Graph<T>::add_edge(const T& v1, const T& v2, unsigned weight) {   
    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    vertices[v1].add_edge(vertices[v2], weight);
    if (vertices[v2].add_edge(vertices[v1], weight))    // if the edge did not already exist
        no_edges += 2;
}


template <typename T>
void Graph<T>::remove_edge(const T& v1, const T& v2) {
    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    if (!vertices[v1].is_neighboor(vertices[v2]))
        throw std::runtime_error("vertices are not connected");

    vertices[v1].remove_edge(vertices[v2]);
    vertices[v2].remove_edge(vertices[v1]);
    no_edges -= 2;
}


template <typename T>
void Graph<T>::change_weight(const T& v1, const T& v2, unsigned new_weight) {
    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    if (!vertices[v1].is_neighboor(vertices[v2]))
        throw std::runtime_error("vertices are not connected");

    vertices[v1].change_weight(v2, new_weight);
    vertices[v2].change_weight(v1, new_weight);
}


template <typename T>
unsigned Graph<T>::get_weight(const T& v1, const T& v2) {    
    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    if (!vertices[v2].is_neighboor(vertices[v1]))
        throw std::runtime_error("vertices are not connected");

    return vertices[v1].get_weight(vertices[v2]);
}


template <typename T>
Vector<T> Graph<T>::get_vertices() {
    Vector<T> to_return;
    for (auto& v : vertices)
        to_return.push_back(v.first);        
    return to_return;
}


template <typename T>
Vector<T> Graph<T>::get_neighboors(const T& vertex) {
    if (vertices.find(vertex) == vertices.end())
        throw std::invalid_argument("no such vertex in the graph");

    Vector<T> neighboors;
    for (auto& v : vertices[vertex].get_neighboors())
        neighboors.push_back((v.get_dest()).get_data());

    return neighboors;  // return a copy of the neighboors, the user is only allowed to modify the graph through its methodata_structures
}


template <typename T>
class Graph<T>::Vertex {
private:
    T data;
    Vector<Edge> neighboors;
public:
    Vertex() = default;

    explicit Vertex(const T& in_data) : data{in_data} {}         

    // Returns true if a new edge was inserted, otherwise it updates its weight and returns false
    bool add_edge(Vertex& dest, unsigned weight) { 
        Edge temp{dest};
        for (auto& e : neighboors) {
            if (e == temp) {
                e.set_weight(weight);
                return false;
            }
        }
        neighboors.push_back(Edge{dest, weight});
        return true;
    }

    void remove_edge(Vertex& vertex) {
        Edge temp{vertex};
        for (auto iter = neighboors.begin(); iter != neighboors.end(); ++iter) {
            if (*iter == temp) {
                neighboors.erase(iter);
                return;
            }
        }
    }

    bool is_neighboor(Vertex& vertex) {
        Edge temp{vertex};
        for (auto& e : neighboors)
            if (e == temp) 
                return true;
        return false;
    }

    T& get_data() { return data; }

    Vector<Edge>& get_neighboors() { return neighboors; }

    void change_weight(const T& data, unsigned new_weight) {
        for (auto& e : neighboors)
            if (e.get_dest().get_data() == data)
                e.set_weight(new_weight);
    }

    unsigned get_weight(const Vertex& vertex) {
        for (auto& e : neighboors)
            if (e.get_dest() == vertex)
                return e.get_weight();
        throw std::runtime_error("vertices are not connected");
    }

    bool operator<(const Vertex& rhs) const {
        return data < rhs.data;
    }

    // Graph does not support duplicate vertices, so we need only to check if their data is the same
    bool operator==(const Vertex& rhs) const {
        return data == rhs.data;
    }
};


template <typename T>
class Graph<T>::Edge {
private:
    Vertex* dest;
    unsigned weight;
public:
    Edge() : dest{}, weight{} {}

    Edge(Vertex& in_dest, unsigned in_weight = 0)
        : dest{&in_dest}, weight{in_weight} {}

    bool operator==(const Edge& rhs) const {
        return *dest == *rhs.dest;  // no duplicate vertices
    }
    
    Vertex& get_dest() { return *dest;  }

    unsigned get_weight() const { return weight; }

    void set_weight(unsigned new_weight) { weight = new_weight; }
};

}