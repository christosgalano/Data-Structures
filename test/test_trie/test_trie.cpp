#include "gtest/gtest.h"
#include "Trie.hpp"

using namespace cds;

TEST(Trie, constructors) {
    Trie default_trie;
    EXPECT_EQ(default_trie.size(), 0);
    EXPECT_EQ(default_trie.empty(), true);

    Trie initializer_trie {"dog", "car", "orange", "split"};
    EXPECT_EQ(initializer_trie.size(), 4);
    EXPECT_EQ(initializer_trie.empty(), false);
}

TEST(Trie, insertions) {
    Trie trie;
    
    trie.insert("dog");
    EXPECT_EQ(trie.size(), 1);
    
    trie.insert("dog");
    EXPECT_EQ(trie.size(), 1);
}

TEST(Trie, removals) {
    Trie trie {"dog", "car", "orange", "split"};

    trie.remove("dog");
    EXPECT_EQ(trie.size(), 3);

    trie.remove("apple");
    EXPECT_EQ(trie.size(), 3);
}

TEST(Trie, searches) {
    Trie trie {"dog", "car", "orange", "split"};

    EXPECT_EQ(trie.search("dog"), true);
    EXPECT_EQ(trie.search("cat"), false);

    EXPECT_EQ(trie.starts_with("o"), true);
    EXPECT_EQ(trie.starts_with("w"), false);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}