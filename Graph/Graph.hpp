#include <vector>
#include <unordered_map>
#include <cstddef>
#include <algorithm>
#include <stdexcept>


template <typename T>
class Graph {
private:
    class Vertex {
    private:
        T data;
        std::vector<Vertex*> neighboors;
    public:
        Vertex() : data{} {}

        Vertex(T in_data)
            : data{in_data} {}        
        
        void add_edge(Vertex& vertex) {
            neighboors.push_back(&vertex);
        }

        void remove_edge(const Vertex& vertex) {
            auto iter = std::find(neighboors.begin(), neighboors.end(), &vertex);
            if (iter != neighboors.end())
                neighboors.erase(iter);
        }

        bool is_neighboor(const Vertex& vertex) {
            auto iter = std::find(neighboors.begin(), neighboors.end(), &vertex);
            return iter != neighboors.end();
        }

        bool operator<(const Vertex& rhs) const {
            return data < rhs.data;
        }

        std::vector<Vertex*>& get_neighboors() {
            return neighboors;
        }

        T& get_data() {
            return data;
        }
    };

    std::unordered_map<T, Vertex> vertices;

    std::size_t no_vertices {};
    std::size_t no_edges    {};

public:
    Graph() = default;

    std::size_t get_no_vertices() const { return no_vertices; }
    std::size_t get_no_edges()    const { return no_edges;    }

    void add_vertex(T value) {
        vertices[value] = Vertex(value);
        ++no_vertices;
    }

    void remove_vertex(T value) {
        auto iter = vertices.find(value);
        if (iter == vertices.end())
            throw std::invalid_argument("No such vertex in the graph");

        for (auto neighboor : iter->second.get_neighboors())
            vertices[neighboor->get_data()].remove_edge(iter->second);

        vertices.erase(iter);
        --no_vertices;
    }

    void add_edge(T v1, T v2) {
        if (v1 == v2)
            return;
        
        if ((vertices.find(v1) == vertices.end()) || (vertices.find(v2) == vertices.end()))
            throw std::invalid_argument("No such vertex/vertices in the graph");

        vertices[v1].add_edge(vertices[v2]);
        vertices[v2].add_edge(vertices[v1]);
    
        ++no_edges;
    }
    
    void remove_edge(T v1, T v2) {
        if (v1 == v2)
            return;
        
        if ((vertices.find(v1) == vertices.end()) || (vertices.find(v2) == vertices.end()))
            throw std::invalid_argument("No such vertex/vertices in the graph");

        if (!vertices[v1].is_neighboor(vertices[v2]))
            throw std::logic_error("Vertices are not connected");

        vertices[v1].remove_edge(vertices[v2]);
        vertices[v2].remove_edge(vertices[v1]);

        --no_edges;
    }

    std::vector<T> get_vertices() {
        std::vector<T> to_return;
        for (auto v : vertices)
            to_return.push_back(v.first);
        
        return to_return;
    }

    std::vector<T> get_neighboors(const T& vertex) {
        if (vertices.find(vertex) == vertices.end())
            throw std::invalid_argument("No such vertex in the graph");

        std::vector<T> neighboors;
        for (auto v : vertices[vertex].get_neighboors())
            neighboors.push_back(v->get_data());

        return neighboors;
    }
};

// TODO : add comments, create class Edge with weights, shortest_path function, make the graph directed