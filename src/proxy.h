#pragma once

#include <memory>
#include <vector>

using Indexes = std::vector<size_t>;

template <typename V>
class IProxy {
public:
    ~IProxy() = default;
    virtual void update(Indexes&& indexes, V&& elem) = 0;
    virtual V get(Indexes&& indexes) const = 0;
};


template <typename V>
class Proxy : public IProxy<V> {
public:
    Proxy(IProxy<V> * prevProxyPtr);
    
    Proxy<V>& operator=(V&& elem);
    Proxy<V>& operator[](std::size_t index);
    operator V();
    
    void update(Indexes&& indexes, V&& elem) override;
    V get(Indexes&& indexes) const override;
    
    void addIndex(size_t index);
    void reserveIndexes(size_t size);
private:
    IProxy<V> * m_subjectPtr;
    Indexes m_indexes;
};


template <typename V>
Proxy<V>::Proxy(IProxy<V> * subjectPtr) : m_subjectPtr{subjectPtr} {}


template <typename V>
Proxy<V>& Proxy<V>::operator[](std::size_t index) {
    addIndex(index);
    return *this;
}


template <typename V>
Proxy<V>& Proxy<V>::operator=(V&& elem) {
    update(std::move(m_indexes), std::move(elem));
    return *this;
}


template <typename V>
void Proxy<V>::update(Indexes&& indexes, V&& elem) {
    m_subjectPtr->update(std::move(indexes), std::move(elem));
}


template <typename V>
V Proxy<V>::get(Indexes&& indexes) const {
    return m_subjectPtr->get(std::move(indexes));
}


template <typename V>
Proxy<V>::operator V() {
    return get(std::move(m_indexes));
}


template <typename V>
void Proxy<V>::addIndex(size_t index) {
    m_indexes.push_back(index);
}


template <typename V>
void Proxy<V>::reserveIndexes(size_t size) {
    m_indexes.reserve(size);
}
