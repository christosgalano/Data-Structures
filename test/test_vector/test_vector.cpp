#include "gtest/gtest.h"
#include "Vector.hpp"

#include <iostream>

using namespace cds;

TEST(Vector, constructors) {
    Vector<int> default_vector;
    EXPECT_EQ(default_vector.size(), 0);
    EXPECT_EQ(default_vector.capacity(), 10);

    Vector<int> vector_size(5);
    EXPECT_EQ(vector_size.size(), 5);
    EXPECT_EQ(vector_size.capacity(), 10);

    Vector<int> vector_fill(5, 1);
    EXPECT_EQ(vector_fill.size(), 5);
    EXPECT_EQ(vector_fill.capacity(), 10);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(vector_fill[i], 1);

    Vector<int> vector_initializer_list {1, 2, 3, 4, 5};
    EXPECT_EQ(vector_initializer_list.size(), 5);
    EXPECT_EQ(vector_initializer_list.capacity(), 10);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(vector_initializer_list[i], i+1);

    Vector<int> copy_vector(vector_fill);
    EXPECT_EQ(copy_vector.size(), 5);
    EXPECT_EQ(copy_vector.empty(), false);
    EXPECT_TRUE(copy_vector == vector_fill);
    
    Vector<int> move_vector(std::move(vector_fill));
    EXPECT_EQ(move_vector.size(), 5);
    EXPECT_EQ(vector_fill.empty(), true);
}

TEST(Vector, insertions) {
    Vector<int> vector;
    for (int i = 0; i < 11; ++i)
        vector.push_back(i);
    EXPECT_EQ(vector.size(), 11);
    EXPECT_EQ(vector.capacity(), 20);

    vector.insert(vector.begin(), 100);
    EXPECT_EQ(vector[0], 100);
    EXPECT_EQ(vector.size(), 12);
    EXPECT_EQ(vector.capacity(), 20);
}

TEST(Vector, removals) {
    Vector<int> vector {1, 2, 3, 4, 5};
    
    vector.pop_back();
    EXPECT_EQ(vector[3], 4);

    EXPECT_EQ(vector.size(), 4);

    vector.erase(vector.begin());
    EXPECT_EQ(vector[0], 2);
    EXPECT_EQ(vector.size(), 3);

    vector.clear();
    EXPECT_EQ(vector.size(), 0);
    EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector, access) {
    Vector<int> vector;

    vector.push_back(10);
    EXPECT_EQ(vector.front(), 10);
    EXPECT_EQ(vector.at(0), 10);
    EXPECT_EQ(vector.back(), 10);

    vector.pop_back();
    
    try {
        vector.front();
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "vector is empty");
    }
    try {
        vector.back();
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "vector is empty");
    }
    try {
        vector.at(0);
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "vector is empty");
    }

    vector.push_back(10);
    try {
        vector.at(1);
    }
    catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        EXPECT_TRUE(msg == "invalid index");
    }
}

TEST(Vector, find) {
    Vector<int> vector {1, 2, 3, 4, 5};
    
    Vector<int>::iterator found = vector.find(4);
    EXPECT_EQ(*found,  4);

    Vector<int>::iterator not_found = vector.find(0);
    EXPECT_EQ(not_found, vector.end());
}

TEST(Vector, overloads) {
    Vector<int> vector;
    for (int i = 0; i < 11; ++i)
        vector.push_back(i);

    Vector<int> copy_vector = vector;
    EXPECT_EQ(copy_vector.size(), 11);
    EXPECT_EQ(copy_vector.capacity(), 20);
    EXPECT_TRUE(copy_vector == vector);

    Vector<int> move_vector = std::move(vector);
    EXPECT_EQ(move_vector.size(), 11);
    EXPECT_EQ(move_vector.capacity(), 20);
    EXPECT_EQ(vector.empty(), true);
    EXPECT_EQ(vector.capacity(), 10);
}

TEST(Vector, swap) {
    Vector<int> vector_1 {1, 1, 1};
    Vector<int> vector_2 {0, 0};

    vector_1.swap(vector_2);
    
    EXPECT_EQ(vector_2.size(), 3);
    for (auto val : vector_2)
        EXPECT_EQ(val, 1);

    EXPECT_EQ(vector_1.size(), 2);
    for (auto val : vector_1)
        EXPECT_EQ(val, 0);
}

TEST(Vector, reserve) {
    Vector<int> vec_reserve;

    vec_reserve.reserve(53);
    EXPECT_EQ(vec_reserve.size(), 0);
    EXPECT_EQ(vec_reserve.capacity(), 53);
   
    vec_reserve.reserve(10);
    EXPECT_EQ(vec_reserve.size(), 0);
    EXPECT_EQ(vec_reserve.capacity(), 53);
}

TEST(Vector, resize) {
    Vector<int> vec_resize(25);
    EXPECT_EQ(vec_resize.size(), 25);
    EXPECT_EQ(vec_resize.capacity(), 25);

    vec_resize.resize(10);
    EXPECT_EQ(vec_resize.size(), 10);
    EXPECT_EQ(vec_resize.capacity(), 25);
    
    vec_resize.resize(20, 100);
    EXPECT_EQ(vec_resize.size(), 20);
    EXPECT_EQ(vec_resize.capacity(), 25);
    for (int i = 10; i < vec_resize.size(); ++i)
        EXPECT_EQ(vec_resize[i], 100);

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}