/*!
 @file
 @brief заголовочный класс, содержащий описание и реализацию
 прокси класса, который осуществляет чтение и добаление элемента в матрицу
 */

#pragma once

#include <memory>
#include <vector>

/// @brief сокращение для набора индексов
using Indexes = std::vector<size_t>;

/*!
 @brief интерфейс проски класса
 @details
 @tparam V тип хранимого в матрице элемента
 */
template <typename V>
class IProxy {
public:
    ~IProxy() = default;
    virtual void update(Indexes&& indexes, V&& elem) = 0; ///< Передает индексы и значение элемента
    virtual V get(Indexes&& indexes) const = 0;           ///< Запрашивает значение элемента по индексам
};


/*!
 @brief прокси класс, который будет возвращен объектом Matrix<T, Default, N>
 при вызове Matrix<T, Default, N>::operator[]
 @details
 @tparam V тип хранимого в матрице элемента
 */
template <typename V>
class Proxy : public IProxy<V> {
public:
    Proxy(IProxy<V> * prevProxyPtr);
    
    Proxy<V>& operator=(V&& elem);
    Proxy<V>& operator[](std::size_t index);
    operator V();
    
    void update(Indexes&& indexes, V&& elem) override; ///< Передает индексы и значение элемента в m_subjectPtr
    V get(Indexes&& indexes) const override;           ///< Запрашивает значение элемента по индексам у m_subjectPtr
    void addIndex(size_t index);                       ///< Добавляет индекс в m_indexes
    void reserveIndexes(size_t index_count);           ///< Резеривирует память под нужное количество индексов
private:
    IProxy<V> * m_subjectPtr; ///<  Указатель на объект-создатель
    Indexes m_indexes;        ///< Набор индексов
};


template <typename V>
Proxy<V>::Proxy(IProxy<V> * subjectPtr) : m_subjectPtr{subjectPtr} {}


/*!
 Принимет и сохраняет индекс, вовзращает ссылку на себя
 @param index Индекс
 @return Ссылку на себя
 */
template <typename V>
Proxy<V>& Proxy<V>::operator[](std::size_t index) {
    addIndex(index);
    return *this;
}


/*!
 Инициирует обновление объекта-создателя
 @param elem Элемент для записи в объект-создатель
 @return Ссылку на себя
 */
template <typename V>
Proxy<V>& Proxy<V>::operator=(V&& elem) {
    update(std::move(m_indexes), std::move(elem));
    return *this;
}


/*!
 Обновляет объект-создатель
 @param indexes  Набор индексов
 @param elem Элемент
 */
template <typename V>
void Proxy<V>::update(Indexes&& indexes, V&& elem) {
    m_subjectPtr->update(std::move(indexes), std::move(elem));
}

/*!
 Запрашивет элемент у объекта-создателя
 @param indexes Набор индексов
 @return Элемент
 */
template <typename V>
V Proxy<V>::get(Indexes&& indexes) const {
    return m_subjectPtr->get(std::move(indexes));
}


/*!
 Инициирует чтение элемента из объекта-создателя
 @return Элемент
 */
template <typename V>
Proxy<V>::operator V() {
    return get(std::move(m_indexes));
}


/*!
 Добавляет  переданный индекс в набор индексов
 @param index Индекс
 */
template <typename V>
void Proxy<V>::addIndex(size_t index) {
    m_indexes.push_back(index);
}


/*!
 Резервирует память под набор индексов
 @param index_count размер набора индексов
 */
template <typename V>
void Proxy<V>::reserveIndexes(size_t index_count) {
    m_indexes.reserve(index_count);
}
