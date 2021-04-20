#include "gtest/gtest.h"
#include "Map.hpp"
#include <string>

using namespace cds;

TEST(Map, constructors) {
    Map<int, std::string> default_map;
    EXPECT_EQ(default_map.size(), 0);
    EXPECT_EQ(default_map.empty(), true);

    Map<int, std::string> map_initializer_list {{1, "Bob"},  {2, "Alice"}};
    EXPECT_EQ(map_initializer_list.size(), 2);
    EXPECT_EQ(map_initializer_list.empty(), false);
    Map<int, std::string>::iterator iter = map_initializer_list.begin();
    EXPECT_EQ(iter->first, 1);
    EXPECT_EQ(iter->second, "Bob");
    ++iter;
    EXPECT_EQ(iter->first, 2);
    EXPECT_EQ(iter->second, "Alice");

    Map<int, std::string> copy_map(map_initializer_list);
    EXPECT_EQ(copy_map.size(), 2);
    iter = copy_map.begin();
    EXPECT_EQ(iter->first, 1);
    EXPECT_EQ(iter->second, "Bob");
    ++iter;
    EXPECT_EQ(iter->first, 2);
    EXPECT_EQ(iter->second, "Alice");

    Map<int, std::string> move_map(std::move(map_initializer_list));
    EXPECT_EQ(move_map.size(), 2);
    EXPECT_EQ(map_initializer_list.empty(), true);
    iter = move_map.begin();
    EXPECT_EQ(iter->first, 1);
    EXPECT_EQ(iter->second, "Bob");
    ++iter;
    EXPECT_EQ(iter->first, 2);
    EXPECT_EQ(iter->second, "Alice");
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}