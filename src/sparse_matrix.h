#pragma once

#include "proxy.h"
#include "data.h"

#include <map>
#include <list>
#include <tuple>
#include <memory>
#include <vector>


using Indexes = std::vector<size_t>;


template <typename T, T Default, size_t N>
class Matrix : public IProxy<T> {
public:
    using Iterator = typename Data<T, N>::It;
    
    Proxy<T> operator[](std::size_t);
    
    void update(Indexes&& indexes, T&& elem) override;
    T get(Indexes&& indexes) const override;
    
    Iterator begin();
    Iterator end();
    size_t size() const;
private:
    Data<T, N> m_data;
};


template <typename T, T Default, size_t N>
void Matrix<T, Default, N>::update(Indexes&& indexes, T&& elem) {
    const auto key = m_data.makeKey(indexes);
    const bool is_default = elem == Default;
    const bool exists     = m_data.contains(key);

    // 1. если пришло    значение по умолчанию и элемент с такими индексами    существует
    if (is_default &&  exists) {
        m_data.erase(key);
    }
    // 2. если пришло    значение по умолчанию и элемент с такими индексами НЕ существует
    //  -- ничего не делаем
    
    // 3. если пришло НЕ значение по умолчанию и элемент с такими индексами НЕ существует
    else if (!is_default && !exists) {
        m_data.insert(key, elem);
    }
    // 4. если пришло НЕ значение по умолчанию и элемент с такими индексами    существует
    else if (!is_default && exists) {
        m_data.erase(key);
        m_data.insert(key, elem);
    }
}


template <typename T, T Default, size_t N>
T Matrix<T, Default, N>::get(Indexes&& indexes) const {
    const auto key  = m_data.makeKey(indexes);
    const auto [status, elem] = m_data.find(key);
    
    switch (status) {
        case Data<T, N>::FindStatus::NOT_FOUND:
            // 1. элемент с такими индексами существует
            return Default;
            break;
        case Data<T, N>::FindStatus::FOUND:
            // 2. элемент с такими индексами не существует
            return elem;
            break;
    }
}


template <typename T, T Default, size_t N>
size_t Matrix<T, Default, N>::size() const {
    return m_data.size();
}


template <typename T, T Default, size_t N>
Proxy<T> Matrix<T, Default, N>::operator[](std::size_t index) {
    Proxy proxy = Proxy<T>{this};
    proxy.addIndex(index);
    proxy.reserveIndexes(N);
    return proxy;
}


template <typename T, T Default, size_t N>
typename Matrix<T, Default, N>::Iterator Matrix<T, Default, N>::begin() {
    return m_data.begin();
}


template <typename T, T Default, size_t N>
typename Matrix<T, Default, N>::Iterator Matrix<T, Default, N>::end() {
    return m_data.end();
}


template <typename T, T Default = 0> using Matrix2D = Matrix<T, Default, 2>;
template <typename T, T Default = 0> using Matrix3D = Matrix<T, Default, 3>;
