#include "gtest/gtest.h"
#include "Map.hpp"
#include <string>

using namespace cds;

TEST(Map, constructors) {
    Map<int, std::string> default_map;
    EXPECT_EQ(default_map.size(), 0);
    EXPECT_EQ(default_map.empty(), true);

    Map<int, std::string> initializer_map {{1, "Bob"},  {2, "Alice"}};
    EXPECT_EQ(initializer_map.size(), 2);
    EXPECT_EQ(initializer_map.empty(), false);
    Map<int, std::string>::iterator iter = initializer_map.begin();
    EXPECT_EQ(iter->first, 1);
    EXPECT_EQ(iter->second, "Bob");
    ++iter;
    EXPECT_EQ(iter->first, 2);
    EXPECT_EQ(iter->second, "Alice");

    Map<int, std::string> copy_map(initializer_map);
    EXPECT_EQ(copy_map.size(), 2);
    iter = copy_map.begin();
    EXPECT_EQ(iter->first, 1);
    EXPECT_EQ(iter->second, "Bob");
    ++iter;
    EXPECT_EQ(iter->first, 2);
    EXPECT_EQ(iter->second, "Alice");

    Map<int, std::string> move_map(std::move(initializer_map));
    EXPECT_EQ(move_map.size(), 2);
    EXPECT_EQ(initializer_map.empty(), true);
    iter = move_map.begin();
    EXPECT_EQ(iter->first, 1);
    EXPECT_EQ(iter->second, "Bob");
    ++iter;
    EXPECT_EQ(iter->first, 2);
    EXPECT_EQ(iter->second, "Alice");
}

TEST(Map, rehash) {
    Map<int, int> map;
    for (int i = 0; i < 54; ++i)
        map.insert(i, i);
    
    EXPECT_EQ(map.size(), 54);
    for (int i = 0; i < 54; ++i)
        EXPECT_EQ(map[i], i);   
}

TEST(Map, insertions) {
    Map<int, std::string> map;
    
    map.insert(1, "Chris");
    map.insert(120, "Bob");
    map.insert(53, "Anna");
    map.insert(200, "Alice");
    EXPECT_EQ(map.size(), 4);
    
    map[2] = "John";
    EXPECT_EQ(map.size(), 5);

    map[2] = "New John";
    EXPECT_EQ(map.size(), 5);
}

TEST(Map, removals) {
    Map<int, std::string> map {{1, "Bob"},  {2, "Alice"}};

    try {
        map.remove(0);
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg ==  "no such key in the map");
    }

    map.remove(1);
    EXPECT_EQ(map.size(), 1);

    map.clear();
    EXPECT_EQ(map.size(), 0);
}   

TEST(Map, find) {
    Map<int, std::string> map {{1, "Bob"},  {2, "Alice"}};
    
    auto found = map.find(1);
    EXPECT_EQ(found->first, 1);
    EXPECT_EQ(found->second, "Bob");

    auto not_found = map.find(0);
    EXPECT_EQ(not_found, map.end());
}

TEST(Map, overloads) {
    Map<int, std::string> map;
    for (int i = 0; i < 11; ++i)
        map.insert(i, "A" + std::to_string(i));

    Map<int, std::string> copy_map = map;
    EXPECT_EQ(copy_map.size(), 11);
    EXPECT_TRUE(copy_map == map);

    Map<int, std::string> move_map = std::move(map);
    EXPECT_EQ(move_map.size(), 11);
    EXPECT_EQ(map.empty(), true);

    EXPECT_EQ(move_map[0], "A0");

    move_map[100] = "A100";
    EXPECT_EQ(move_map.size(), 12);
    EXPECT_EQ(move_map[100], "A100");
}

TEST(Map, swap) {
    Map<int, std::string> map_1 {{1, "Bob"},  {20, "Alice"}};
    Map<int, std::string> map_2 {{10, "John"}};

    map_1.swap(map_2);
    
    EXPECT_EQ(map_2.size(), 2);
    EXPECT_EQ(map_2[20], "Alice");
    EXPECT_EQ(map_2[1], "Bob");

    EXPECT_EQ(map_1.size(), 1);
    EXPECT_EQ(map_1[10], "John");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}