#include <string>
#include <gtest/gtest.h>
#include "data_structures.hpp"

using namespace data_structures;

TEST(Graph, constructors) {
    Graph<int> default_graph;
    EXPECT_EQ(default_graph.get_no_vertices(), 0);

    Graph<int> initializer_graph {1, 2, 3, 4};
    EXPECT_EQ(initializer_graph.get_no_vertices(), 4);
    EXPECT_EQ(initializer_graph.get_no_edges(), 0);
}

TEST(Graph, insertions) {
    Graph<int> graph;

    graph.add_vertex(10);
    EXPECT_EQ(graph.get_no_vertices(), 1);
    auto vertices = graph.get_vertices();
    EXPECT_EQ(vertices[0], 10);

    graph.add_vertex(20);

    graph.add_edge(10, 20, 100);
    EXPECT_EQ(graph.get_no_edges(), 2);  // graph is undirected
    auto neighboors = graph.get_neighboors(10);
    EXPECT_EQ(neighboors[0], 20);

    graph.add_edge(10, 20, 400);  // simply update the weight
    EXPECT_EQ(graph.get_no_edges(), 2);
    neighboors = graph.get_neighboors(20);
    EXPECT_EQ(neighboors[0], 10);

    try {
        graph.add_edge(1, 20, 100);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "no such vertex/vertices in the graph");
    }
}

TEST(Graph, removals) {
    Graph<int> graph {1, 2, 3, 4};

    graph.add_edge(1, 2, 100);
    graph.add_edge(1, 3, 100);
    graph.add_edge(1, 4, 100);
    graph.add_edge(2, 4, 200);
    graph.add_edge(3, 4, 300);

    graph.remove_edge(1, 2);
    EXPECT_EQ(graph.get_no_edges(), 8);

    graph.remove_vertex(4);
    EXPECT_EQ(graph.get_no_vertices(), 3);
    EXPECT_EQ(graph.get_no_edges(), 2);

    try {
        graph.remove_vertex(5);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "no such vertex in the graph");
    }

    try {
        graph.remove_edge(1, 5);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "no such vertex/vertices in the graph");
    }

    try {
        graph.remove_edge(2, 3);
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "vertices are not connected");
    }
}

TEST(Graph, weights) {
    Graph<int> graph {1, 2, 3};

    graph.add_edge(1, 2, 100);
    EXPECT_EQ(graph.get_weight(1, 2), 100);
    graph.add_edge(1, 2, 400);
    EXPECT_EQ(graph.get_weight(1, 2), 400);
    
    graph.add_edge(1, 3, 100);
    graph.change_weight(1, 3, 500);
    EXPECT_EQ(graph.get_weight(1, 3), 500);

    try {
        graph.get_weight(1, 5);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "no such vertex/vertices in the graph");
    }

    try {
        graph.get_weight(2, 3);
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "vertices are not connected");
    }

    try {
        graph.change_weight(1, 5, 100);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "no such vertex/vertices in the graph");
    }

    try {
        graph.change_weight(2, 3, 100);
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_EQ(msg, "vertices are not connected");
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}