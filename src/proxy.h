/*!
 @file
 @brief заголовочный класс, содержащий описание и реализацию
 прокси класса, который осуществляет чтение и добаление элемента в матрицу
 */

#pragma once

#include "indexes.h"
#include <memory>
#include <string>
#include <array>
#include <stdexcept>
#include <iostream>

/*!
 @brief интерфейс проски класса
 @details
 @tparam V тип хранимого в матрице элемента
 @tparam N размерность матрицы
 */
template <typename V, size_t N>
class IProxy {
public:
    ~IProxy() = default;
    virtual void update(const Indexes<N>& indexes, const V& elem) = 0; ///< Передает индексы и значение элемента
    virtual V get(const Indexes<N>& indexes) const = 0;                ///< Запрашивает значение элемента по индексам
};


/*!
 @brief прокси класс, который будет возвращен объектом Matrix<T, Default, N>
 при вызове Matrix<T, Default, N>::operator[]
 @details
 @tparam V тип хранимого в матрице элемента
 */
template <typename V, size_t N>
class Proxy : public IProxy<V, N> {
public:
    Proxy(IProxy<V, N> * prevProxyPtr);
    
    Proxy<V, N>& operator=(const V& elem);
    Proxy<V, N>& operator[](std::size_t index);
    operator V();
    
    void update(const Indexes<N>& indexes, const V& elem) override; ///< Передает индексы и значение элемента в m_subjectPtr
    V get(const Indexes<N>& indexes) const override;           ///< Запрашивает значение элемента по индексам у m_subjectPtr
    void addIndex(size_t index);                       ///< Добавляет индекс в m_indexes
private:
    IProxy<V, N> * m_subjectPtr; ///<  Указатель на объект-создатель
    Indexes<N> m_indexes;        ///< Набор индексов
    size_t m_counter = 0;        ///< Счетчик переданных индексов
};


template <typename V, size_t N>
Proxy<V, N>::Proxy(IProxy<V, N> * subjectPtr) : m_subjectPtr{subjectPtr} {}


/*!
 Принимет и сохраняет индекс, вовзращает ссылку на себя
 @param index Индекс
 @return Ссылку на себя
 */
template <typename V, size_t N>
Proxy<V, N>& Proxy<V, N>::operator[](std::size_t index) {
    addIndex(index);
    return *this;
}


/*!
 Инициирует обновление объекта-создателя
 @param elem Элемент для записи в объект-создатель
 @return Ссылку на себя
 */
template <typename V, size_t N>
Proxy<V, N>& Proxy<V, N>::operator=(const V& elem) {
    update(m_indexes, elem);
    return *this;
}


/*!
 Обновляет объект-создатель
 @param indexes  Набор индексов
 @param elem Элемент
 @throw std::runtime_error Если передано слишком мало индексов
 */
template <typename V, size_t N>
void Proxy<V, N>::update(const Indexes<N>& indexes, const V& elem) {
    if (m_counter < N) {
        std::string error_message = "Too few indexes";
        throw std::runtime_error(error_message);
    }
    m_subjectPtr->update(indexes, elem);
}

/*!
 Запрашивет элемент у объекта-создателя
 @param indexes Набор индексов
 @return Элемент
 */
template <typename V, size_t N>
V Proxy<V, N>::get(const Indexes<N>& indexes) const {
    return m_subjectPtr->get(indexes);
}


/*!
 Инициирует чтение элемента из объекта-создателя
 @return Элемент
 */
template <typename V, size_t N>
Proxy<V, N>::operator V() {
    return get(m_indexes);
}


/*!
 Добавляет  переданный индекс в набор индексов
 @param index Индекс
 @throw std::runtime_error Если произошла попытка добавления лишнего индекса
 */
template <typename V, size_t N>
void Proxy<V, N>::addIndex(size_t index) {
    if (m_counter >= N) {
        std::string error_message = "Too many indexes";
        throw std::runtime_error(error_message);
    }
    
    m_indexes[m_counter++] = index;
}
