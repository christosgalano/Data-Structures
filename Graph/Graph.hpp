#include <vector>
#include <unordered_map>
#include <cstddef>
#include <algorithm>
#include <stdexcept>


template <typename T>
class Graph {
private:
    class Vertex;
    class Edge;

    std::unordered_map<T, Vertex> vertices;
    std::size_t no_vertices {};
    std::size_t no_edges    {};

    bool validate(const T& v1, const T& v2) const;

public:
    Graph() = default;

    std::size_t get_no_vertices() const { return no_vertices; }
    std::size_t get_no_edges()    const { return no_edges;    }

    void add_vertex(const T& value);

    void remove_vertex(const T& value);

    void add_edge(const T& v1, const T& v2, unsigned weight);
    
    void remove_edge(const T& v1, const T& v2);

    void change_weight(const T& v1, const T& v2, unsigned new_weight);

    unsigned get_weight(const T& v1, const T& v2);

    std::vector<T> get_vertices();

    std::vector<T> get_neighboors(const T& vertex);
};


template <typename T>
bool Graph<T>::validate(const T& v1, const T& v2) const {
    return ( vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end() ) ? false : true;
}

template <typename T>
void Graph<T>::add_vertex(const T& value) {
    vertices[value] = Vertex(value);
    ++no_vertices;
}

template <typename T>
void Graph<T>::remove_vertex(const T& value) {
    auto iter = vertices.find(value);
    if (iter == vertices.end())
        throw std::invalid_argument("no such vertex in the graph");

    for (auto neighboor : iter->second.get_neighboors())
        vertices[(neighboor.get_dest()).get_data()].remove_edge(iter->second);

    vertices.erase(iter);
    --no_vertices;
}

template <typename T>
void Graph<T>::add_edge(const T& v1, const T& v2, unsigned weight) {
    if (v1 == v2)
        return;        

    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    vertices[v1].add_edge(vertices[v2], weight);
    vertices[v2].add_edge(vertices[v1], weight);    
    ++no_edges;
}

template <typename T>
void Graph<T>::remove_edge(const T& v1, const T& v2) {
    if (v1 == v2)
        return;        

    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    if (!vertices[v1].is_neighboor(vertices[v2]))
        throw std::logic_error("vertices are not connected");

    vertices[v1].remove_edge(vertices[v2]);
    vertices[v2].remove_edge(vertices[v1]);
    --no_edges;
}

template <typename T>
void Graph<T>::change_weight(const T& v1, const T& v2, unsigned new_weight) {
    if (v1 == v2)
        return;
    
    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    if (!vertices[v2].is_neighboor(vertices[v1]))
        throw std::logic_error("vertices are not connected");

    vertices[v1].change_weight(v2, new_weight);
    vertices[v2].change_weight(v1, new_weight);
}

template <typename T>
unsigned Graph<T>::get_weight(const T& v1, const T& v2) {
    if (v1 == v2)
        return 0;
    
    if (!validate(v1, v2))
        throw std::invalid_argument("no such vertex/vertices in the graph");

    if (!vertices[v2].is_neighboor(vertices[v1]))
        throw std::logic_error("vertices are not connected");

    return vertices[v1].get_weight(v2);
}

template <typename T>
std::vector<T> Graph<T>::get_vertices() {
    std::vector<T> to_return;
    for (auto& v : vertices)
        to_return.push_back(v.first);        
    return to_return;
}

template <typename T>
std::vector<T> Graph<T>::get_neighboors(const T& vertex) {
    if (vertices.find(vertex) == vertices.end())
        throw std::invalid_argument("no such vertex in the graph");

    std::vector<T> neighboors;
    for (auto v& : vertices[vertex].get_neighboors())
        neighboors.push_back((v.get_dest()).get_data());

    return neighboors;
}


template <typename T>
class Graph<T>::Vertex {
private:
    T data;
    std::vector<Graph<T>::Edge> neighboors;
public:
    Vertex() : data{} {}
    Vertex(T in_data) : data{in_data} {}         

    void add_edge(Vertex& dest, unsigned weight) {
        neighboors.push_back(Edge{dest, weight});
    }

    void remove_edge(Vertex& vertex) {
        auto iter = std::find(neighboors.begin(), neighboors.end(), Edge{vertex});
        neighboors.erase(iter);
    }

    bool is_neighboor(Vertex& vertex) const {
        auto iter = std::find(neighboors.begin(), neighboors.end(), Edge{vertex});
        return iter != neighboors.end();
    }

    bool operator<(const Vertex& rhs) const {
        return data < rhs.data;
    }

    std::vector<Edge>& get_neighboors() {
        return neighboors;
    }

    T& get_data() {
        return data;
    }

    void change_weight(const T& vertex, unsigned new_weight) {
        for (auto& e : neighboors)
            if (e.get_dest() == vertex)
                e.set_weight(new_weight);   
    }

    unsigned get_weight(const Vertex& vertex) {
        // We have already checked that the vertices are connected
        for (auto& e : neighboors)
            if (e.get_dest() == vertex)
                return e.get_weight();
    }

    // This Graph does not support duplicate vertices, so we need only to check if their data is the same
    bool operator==(const Vertex& rhs) const {
        return data == rhs.data;
    }
};


template <typename T>
class Graph<T>::Edge {
private:
    Vertex*  dest;
    unsigned weight;
public:
    explicit Edge(Vertex& in_dest, unsigned in_weight = 0)
        : dest{&in_dest}, weight{in_weight} {}

    bool operator==(const Edge& rhs) const {
        return *dest == *rhs.dest;
    }
    
    Vertex&    get_dest() const { return *dest;  }
    unsigned get_weight() const { return weight; }

    void set_weight(unsigned new_weight) { weight = new_weight; }
};

// TODO : add comments, shortest_path function, make the graph directed