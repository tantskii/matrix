#include "data.h"

#include "gtest/gtest.h"

#include <vector>


TEST(Data, MakeKey) {
    Data<int, 2> data;
    Indexes<2> indexes = {100, 100};
    
    auto key = data.makeKey(indexes);
    
    ASSERT_EQ(std::make_tuple(100, 100), key);
}


TEST(Data, MakeElement) {
    Data<int, 2> data;
    Indexes<2> indexes = {100, 100};
    int value = 314;
    
    auto key  = data.makeKey(indexes);
    auto elem = data.makeElement(key, value);
    
    ASSERT_EQ(std::make_tuple(100, 100, 314), elem);
}


TEST(Data, Insert) {
    Data<int, 2> data;
    Indexes<2> indexes = {100, 100};
    int value = 314;
    
    auto key  = data.makeKey(indexes);
    data.insert(key, value);
    const auto [status, value_1] = data.find(key);
    const auto [exists, it] = data.contains(key);
    
    ASSERT_TRUE(exists);
    ASSERT_EQ(value_1, value);
}


TEST(Data, InsertTwice) {
    Data<int, 2> data;
    const Indexes<2> indexes = {100, 100};
    const int value_1 = 314;
    const int value_2 = 215;
    
    auto key  = data.makeKey(indexes);
    data.insert(key, value_1);
    data.insert(key, value_2);
    
    const auto [status, value_3] = data.find(key);
    const auto [exists, it] = data.contains(key);
    
    ASSERT_TRUE(exists);
    ASSERT_EQ(value_3, value_2);
}


TEST(Data, Contains) {
    Data<int, 2> data;
    Indexes<2> indexes_1 = {100, 100};
    Indexes<2> indexes_2 = {120, 400};
    int value = 314;
    
    auto key_1  = data.makeKey(indexes_1);
    auto key_2  = data.makeKey(indexes_2);
    data.insert(key_1, value);
    
    const auto [exists_1, it_1] = data.contains(key_1);
    const auto [exists_2, it_2] = data.contains(key_2);
    
    ASSERT_TRUE (exists_1);
    ASSERT_FALSE(exists_2);
}


TEST(Data, Erase) {
    Data<int, 2> data;
    Indexes<2> indexes = {100, 100};
    int value = 314;
    
    auto key  = data.makeKey(indexes);
    data.insert(key, value);
    data.erase(key);
    const auto [status, value_2] = data.find(key);
    const auto [exists, it] = data.contains(key);
    
    ASSERT_FALSE(exists);
    ASSERT_FALSE(value_2);
}


TEST(Data, EraseUninsertedElement) {
    Data<int, 2> data;
    Indexes<2> indexes = {100, 100};
    
    auto key  = data.makeKey(indexes);
    
    ASSERT_THROW(data.erase(key), std::runtime_error);
}


TEST(Data, Find) {
    Data<int, 2> data;
    Indexes<2> indexes_1 = {100, 100};
    Indexes<2> indexes_2 = {120, 400};
    int value = 314;
    
    auto key_1 = data.makeKey(indexes_1);
    auto key_2 = data.makeKey(indexes_2);
    data.insert(key_1, value);
    const auto [status_1, value_1] = data.find(key_1);
    const auto [status_2, value_2] = data.find(key_2);

    ASSERT_EQ(value_1, value);
    ASSERT_EQ(value_2, 0);
}
