#include "gtest/gtest.h"
#include "PriorityQueue.hpp"

using namespace cds;

TEST(PriorityQueue, constructors) {
    PriorityQueue<int> default_pq;
    EXPECT_EQ(default_pq.size(), 0);
    EXPECT_EQ(default_pq.empty(), true);
    
    Vector<int> vec{1, 2, 3};
    PriorityQueue<int> pq_from_vec(vec);
    EXPECT_EQ(pq_from_vec.size(), vec.size());
    EXPECT_EQ(pq_from_vec.top(), 3); // std::less is the default comparator

    PriorityQueue<int> pq_from_temp_vec(Vector<int>{1, 2, 3, 4});
    EXPECT_EQ(pq_from_temp_vec.size(), 4);
    EXPECT_EQ(pq_from_temp_vec.top(), 4);

    auto comp = [] (int a, int b) { return a > b; };    // same as std::greater
    PriorityQueue<int, decltype(comp)> pq_with_custom_comparator(Vector<int>{1, 2, 3, 4}, comp);
    EXPECT_EQ(pq_with_custom_comparator.size(), 4);
    EXPECT_EQ(pq_with_custom_comparator.top(), 1);

    PriorityQueue<int, std::greater<int>> initializer_pq{1, 2, 3, 4};
    EXPECT_EQ(initializer_pq.size(), 4);
    EXPECT_EQ(initializer_pq.top(), 1);

    PriorityQueue<int, std::greater<int>> copy_pq(initializer_pq);
    EXPECT_TRUE(copy_pq == initializer_pq);

    PriorityQueue<int, std::greater<int>> move_pq(std::move(initializer_pq));
    EXPECT_EQ(move_pq.size(), 4);
    EXPECT_EQ(move_pq.top(), 1);
    EXPECT_EQ(initializer_pq.empty(), true);
}

TEST(PriorityQueue, insertions) {
    PriorityQueue<int, std::greater<int>> pq {1, 2, 3};

    pq.insert(0);
    EXPECT_EQ(pq.top(), 0);

    pq.insert(100);
    EXPECT_EQ(pq.top(), 0);
}

TEST(PriorityQueue, removals) {
    Vector<int> vec;
    for (int i = 0; i < 100; ++i)
        vec.push_back(i);

    PriorityQueue<int> pq(vec);
    int val = 99;
    while (!pq.empty()) {
        EXPECT_EQ(pq.top(), val--);
        pq.pop();
    }

    try {
        pq.top();
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "pq is empty");
    }
    try {
        pq.pop();
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "pq is empty");
    }
}

TEST(PriorityQueue, overloads) {
    Vector<int> vec;
    for (int i = 0; i < 11; ++i)
        vec.push_back(i);
    
    PriorityQueue<int> pq(vec);

    PriorityQueue<int> copy_pq = pq;
    EXPECT_EQ(copy_pq.size(), 11);
    EXPECT_TRUE(copy_pq == pq);

    PriorityQueue<int> move_pq = std::move(pq);
    EXPECT_EQ(move_pq.size(), 11);
    EXPECT_EQ(pq.empty(), true);
}

TEST(PriorityQueue, swap) {
    PriorityQueue<int> pq_1 {1, 1, 1};
    PriorityQueue<int> pq_2 {0, 0};

    pq_1.swap(pq_2);
    
    EXPECT_EQ(pq_2.size(), 3);
    while (!pq_2.empty()) {
        EXPECT_EQ(pq_2.top(), 1);
        pq_2.pop();
    }

    EXPECT_EQ(pq_1.size(), 2);
    while (!pq_1.empty()) {
        EXPECT_EQ(pq_1.top(), 0);
        pq_1.pop();
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}