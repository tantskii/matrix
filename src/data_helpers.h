/*!
 @file
 @brief Данный заголовочный файл содержит вспомогательные функии
 класса Data<T, N> описанного в data.h
 */

#pragma once

#include <vector>
#include <tuple>
#include <utility>

using Indexes = std::vector<size_t>;


template<std::size_t... Is>
auto key_type(std::index_sequence<Is...>) {
    return std::make_tuple(Is...);
}


template<typename T, std::size_t... Is>
auto elem_type(std::index_sequence<Is...>, T t) {
    return std::make_tuple(Is..., t);
}


template<std::size_t... I>
auto makeKeyImpl(const Indexes& v, std::index_sequence<I...>) {
    return std::make_tuple(v[I]...);
}


template<typename T, typename Key, std::size_t... I>
auto makeElemImpl(const Key& key, std::index_sequence<I...>, const T& elem) {
    return std::make_tuple(std::get<I>(key)..., elem);
}
