#pragma once

#include "data_helpers.h"

#include <list>
#include <map>


using Indexes = std::vector<size_t>;


template <typename T, size_t N>
class Data {
public:
    enum class FindStatus {FOUND, NOT_FOUND};
    
    using Key      = decltype(key_type (std::make_index_sequence<N>{}));
    using Element  = decltype(elem_type(std::make_index_sequence<N>{}, T{}));
    using It       = typename std::list<Element>::iterator;
    using MapIt    = typename std::map<Key, It>::iterator;
    
    void erase(const Key& key);
    void insert(const Key& key, const T& elem);
    bool contains(const Key& key) const;
    std::pair<FindStatus, T> find(const Key& key) const;
    
    size_t size() const;
    It begin();
    It end();
    
    Key makeKey(const Indexes& indexes) const;
    Element makeElement(const Key& key, const T& elem) const;
private:
    std::list<Element> m_data;
    std::map<Key, It> m_map;
};


template <typename T, size_t N>
void Data<T, N>::erase(const Key& key) {
    auto it = m_map.find(key);
    m_data.erase(it->second);
    m_map.erase(it);
}


template <typename T, size_t N>
void Data<T, N>::insert(const Key& key, const T& elem) {
    m_data.push_back(makeElement(key, elem));
    m_map.emplace(key, std::prev(m_data.end()));
}


template <typename T, size_t N>
bool Data<T, N>::contains(const Key& key) const {
    auto it = m_map.find(key);
    return it != m_map.end();
}


template <typename T, size_t N>
std::pair<typename Data<T, N>::FindStatus, T> Data<T, N>::find(const Key& key) const {
    auto it = m_map.find(key);
    if (it == m_map.end()) {
        return {FindStatus::NOT_FOUND, T{}};
    }
    return {FindStatus::FOUND, std::get<N>(*(it->second))};
}


template <typename T, size_t N>
size_t Data<T, N>::size() const {
    return m_data.size();
}


template <typename T, size_t N>
typename Data<T, N>::Key Data<T, N>::makeKey(const Indexes& v) const {
    return makeKeyImpl(v, std::make_index_sequence<N>{});
}


template <typename T, size_t N>
typename Data<T, N>::Element Data<T, N>::makeElement(const Key& key, const T& elem) const {
    return makeElemImpl(key, std::make_index_sequence<N>{}, elem);
}


template <typename T, size_t N>
typename Data<T, N>::It Data<T, N>::begin() {
    return m_data.begin();
}


template <typename T, size_t N>
typename Data<T, N>::It Data<T, N>::end() {
    return m_data.end();
}
