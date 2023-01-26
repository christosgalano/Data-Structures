#include <string>
#include <gtest/gtest.h>
#include "Data_Structures.hpp"

using namespace data_structures;

TEST(BST, constructors){
    BST<int> default_bst;
    EXPECT_EQ(default_bst.size(), 0);
    EXPECT_EQ(default_bst.empty(), true);

    BST<int> initializer_bst {2, 5, 10};
    EXPECT_EQ(initializer_bst.size(), 3);
    EXPECT_EQ(initializer_bst.empty(), false);
}

TEST(BST, insert) {
    BST<int> bst;

    bst.insert(10);
    EXPECT_EQ(bst.size(), 1);
    EXPECT_EQ(*bst.begin(), 10);

    bst.insert(4);
    bst.insert(8);
    bst.insert(15);
    bst.insert(12);

    auto iter = bst.begin();
    EXPECT_EQ(*iter++,  4);
    EXPECT_EQ(*iter++,  8);
    EXPECT_EQ(*iter++, 10);
    EXPECT_EQ(*iter++, 12);
    EXPECT_EQ(*iter,   15);
}

TEST(BST, removals) {
    BST<int> bst {10, 4, 2, 8, 15, 11, 12, 20};

    bst.remove(11);
    EXPECT_EQ(bst.size(), 7);
    auto iter = bst.cbegin();
    EXPECT_EQ(*iter++,  2);
    EXPECT_EQ(*iter++,  4);
    EXPECT_EQ(*iter++,  8);
    EXPECT_EQ(*iter++, 10);
    EXPECT_EQ(*iter++, 12);
    EXPECT_EQ(*iter++, 15);
    EXPECT_EQ(*iter,   20);

    bst.remove(2);
    EXPECT_EQ(bst.size(), 6);
    iter = bst.cbegin();
    EXPECT_EQ(*iter++,  4);
    EXPECT_EQ(*iter++,  8);
    EXPECT_EQ(*iter++, 10);
    EXPECT_EQ(*iter++, 12);
    EXPECT_EQ(*iter++, 15);
    EXPECT_EQ(*iter,   20);

    bst.remove(10);
    EXPECT_EQ(bst.size(), 5);
    iter = bst.cbegin();
    EXPECT_EQ(*iter++,  4);
    EXPECT_EQ(*iter++,  8);
    EXPECT_EQ(*iter++, 12);
    EXPECT_EQ(*iter++, 15);
    EXPECT_EQ(*iter,   20);

    try {
        bst.remove(100);
    }
    catch (const std::runtime_error& e) {
        std::string msg = "no such value in the BST";
        EXPECT_EQ(e.what(), msg);
    }

    bst.clear();
    EXPECT_EQ(bst.size(), 0);
}

TEST(BST, search) {
    BST<int> bst {10, 4, 2, 8, 15, 11, 12, 20};
    EXPECT_EQ(bst.search(11), true);
    EXPECT_EQ(bst.search(21), false);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}