#include "gtest/gtest.h"
#include "List.hpp"

using namespace cds;

TEST(List, constructors) {
    List<int> default_list;
    EXPECT_EQ(default_list.size(), 0);
    EXPECT_EQ(default_list.empty(), true);

    List<int> initializer_list {1, 2, 3, 4, 5};
    EXPECT_EQ(initializer_list.size(), 5);
    EXPECT_EQ(initializer_list.empty(), false);

    List<int> copy_list(initializer_list);
    EXPECT_EQ(copy_list.size(), 5);
    EXPECT_TRUE(copy_list == initializer_list);
    
    List<int> move_list(std::move(initializer_list));
    EXPECT_EQ(move_list.size(), 5);
    EXPECT_EQ(move_list.empty(), false);
    EXPECT_EQ(initializer_list.size(), 0);
    EXPECT_EQ(initializer_list.empty(), true);
}

TEST(List, insertions) {
    List<int> list;
    for (int i = 0; i < 10; ++i)
      list.push_back(i);

    int v = 0;
    for (auto val : list)
      EXPECT_EQ(val, v++);

    list.clear();

    for (int i = 0; i < 10; ++i)
      list.push_front(i);

    v = 9;
    for (auto val : list)
      EXPECT_EQ(val, v--);

    list.insert(0, 100);
    EXPECT_EQ(*list.begin(), 100);
}

TEST(List, removals) {
    List<int> list {1, 2, 3, 4, 5};
    
    list.pop_front();
    EXPECT_EQ(list.front(), 2);

    list.pop_back();
    EXPECT_EQ(list.back(), 4);

    EXPECT_EQ(list.size(), 3);

    list.remove(1);
    EXPECT_EQ(list.at(1), 4);

    list.clear();
    EXPECT_EQ(list.size(), 0);
}

TEST(List, access) {
    List<int> list;

    list.push_back(10);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.at(0), 10);
    EXPECT_EQ(list.back(), 10);

    list.pop_back();
    
    try {
        list.front();
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "list is empty");
    }
    try {
        list.back();
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "list is empty");
    }
    try {
        list.at(0);
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "list is empty");
    }

    list.push_back(10);
    try {
        list.remove(1);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "invalid index");
    }
}

TEST(List, find) {
    List<int> list {1, 2, 3, 4, 5};
    
    List<int>::iterator found = list.find(4);
    EXPECT_EQ(*found,  4);

    List<int>::iterator not_found = list.find(0);
    EXPECT_EQ(not_found, list.end());
}

TEST(List, overloads) {
    List<int> list {1, 2, 3, 4, 5};

    List<int> copy_list = list;
    EXPECT_EQ(copy_list.size(), 5);
    EXPECT_TRUE(list == copy_list);

    List<int> move_list = std::move(list);
    EXPECT_EQ(move_list.size(), 5);
    EXPECT_EQ(list.empty(), true);
}

TEST(List, swap) {
    List<int> list_1 {1, 1, 1};
    List<int> list_2 {0, 0};

    list_1.swap(list_2);
    
    EXPECT_EQ(list_2.size(), 3);
    for (auto val : list_2)
      EXPECT_EQ(val, 1);

    EXPECT_EQ(list_1.size(), 2);
    for (auto val : list_1)
      EXPECT_EQ(val, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}