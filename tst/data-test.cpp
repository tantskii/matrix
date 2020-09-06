#include "data.h"

#include "gtest/gtest.h"

#include <vector>


TEST(Data, MakeKey) {
    Data<int, 2> data;
    Indexes indexes = {100, 100};
    
    auto key = data.makeKey(indexes);
    
    ASSERT_EQ(std::make_tuple(100, 100), key);
}


TEST(Data, MakeElement) {
    Data<int, 2> data;
    Indexes indexes = {100, 100};
    int value = 314;
    
    auto key  = data.makeKey(indexes);
    auto elem = data.makeElement(key, value);
    
    ASSERT_EQ(std::make_tuple(100, 100, 314), elem);
}


TEST(Data, Insert) {
    Data<int, 2> data;
    Indexes indexes = {100, 100};
    int value = 314;
    
    auto key  = data.makeKey(indexes);
    data.insert(key, value);
    const auto [status, value_1] = data.find(key);
    
    ASSERT_TRUE(data.contains(key));
    ASSERT_EQ(value_1, value);
}


TEST(Data, Contains) {
    Data<int, 2> data;
    Indexes indexes_1 = {100, 100};
    Indexes indexes_2 = {120, 400};
    int value = 314;
    
    auto key_1  = data.makeKey(indexes_1);
    auto key_2  = data.makeKey(indexes_2);
    data.insert(key_1, value);
    
    ASSERT_TRUE (data.contains(key_1));
    ASSERT_FALSE(data.contains(key_2));
}


TEST(Data, Erase) {
    Data<int, 2> data;
    Indexes indexes = {100, 100};
    int value = 314;
    
    auto key  = data.makeKey(indexes);
    data.insert(key, value);
    data.erase(key);
    const auto [status, value_2] = data.find(key);
    
    ASSERT_FALSE(data.contains(key));
    ASSERT_FALSE(value_2);
}


TEST(Data, Find) {
    Data<int, 2> data;
    Indexes indexes_1 = {100, 100};
    Indexes indexes_2 = {120, 400};
    int value = 314;
    
    auto key_1 = data.makeKey(indexes_1);
    auto key_2 = data.makeKey(indexes_2);
    data.insert(key_1, value);
    const auto [status_1, value_1] = data.find(key_1);
    const auto [status_2, value_2] = data.find(key_2);
    
//    ASSERT_TRUE (value_1);
//    ASSERT_FALSE(value_2);
    ASSERT_EQ(value_1, value);
    ASSERT_EQ(value_2, 0);
}
