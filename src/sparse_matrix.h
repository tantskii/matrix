/*!
@file
@brief Заголовочный класс, содержащий определение и реализацию класса бесконечной разреженной матрицы
*/

#pragma once
#include "proxy.h"
#include "data.h"
#include <map>
#include <list>
#include <tuple>
#include <memory>
#include <vector>

/*!
 @brief Целевой класс, реализующий бесконечную n-мерную разреженную матрицу
 @details
 @tparam T тип хранимого элемента
 @tparam Default  значение хранимого элемента по умолчанию
 @tparam N размерность матрицы
 */
template <typename T, T Default, size_t N>
class Matrix : public IProxy<T, N> {
public:
    /// @brief сокращение итератора
    using Iterator = typename Data<T, N>::It;
    
    Proxy<T, N> operator[](std::size_t);
    
    void update(Indexes<N>&& indexes, T&& value) override; ///< Записывает элемент в ячейку с переданными индексами
    T get(Indexes<N>&& indexes) const override;            ///< Считывает элемент из ячейки с переданными индексами
    
    Iterator begin();
    Iterator end();
    size_t size() const; ///< Возвращает количесвто хранимых элементов
private:
    Data<T, N> m_data; ///< Объект-хранитель элементов
};


/*!
 Записывает элемент в ячейку с переданными индексами
 @param indexes  Набор индексов
 @param value Записываемое значение
 */
template <typename T, T Default, size_t N>
void Matrix<T, Default, N>::update(Indexes<N>&& indexes, T&& value) {
    /*
      1. Если пришло    значение по умолчанию и элемент с такими индексами    существует
      -- удаляем этот элемент
      2. Если пришло    значение по умолчанию и элемент с такими индексами НЕ существует
      -- ничего не делаем
      3. Если пришло НЕ значение по умолчанию и элемент с такими индексами НЕ существует
      -- сохраняем этот элемент
      4. Если пришло НЕ значение по умолчанию и элемент с такими индексами    существует
      -- удаляем старое значение по этому ключу и затем добавляем новое с таким же ключом
     */
     
    const auto key = m_data.makeKey(indexes);
    const bool is_default = value == Default;
    const bool exists     = m_data.contains(key);

    if (is_default && exists) {
        // п.1 Значение по умолчанию и элемент с такими индексами существует
        m_data.erase(key);
    } else if (!is_default) {
        // п.3 & п.4 Пришло НЕ значение по умолчанию
        m_data.insert(key, value); // Data<T, N>::insert удалит старое значение,
                                   // в случае, когда элемент по такому ключу уже существует
    }
}


/*!
Считывет элемент в ячейку с переданными индексами
@param indexes  Набор индексов
@return Хранимое значение
*/
template <typename T, T Default, size_t N>
T Matrix<T, Default, N>::get(Indexes<N>&& indexes) const {
    /*
     1. Если элемент с такими индексами    существует
     -- возвращаем его
     2. Если элемент с такими индексами НЕ существует
     -- возвращаем значение по умолчанию
     */
    
    const auto key  = m_data.makeKey(indexes);
    const auto [status, elem] = m_data.find(key);
    
    switch (status) {
        case Data<T, N>::FindStatus::FOUND:
            // п.1
            return elem;
            break;
        case Data<T, N>::FindStatus::NOT_FOUND:
            // п.2
            return Default;
            break;
    }
}


/*!
@return Количество хранимых элементов
*/
template <typename T, T Default, size_t N>
size_t Matrix<T, Default, N>::size() const {
    return m_data.size();
}


/*!
@return Проксирующий класс
*/
template <typename T, T Default, size_t N>
Proxy<T, N> Matrix<T, Default, N>::operator[](std::size_t index) {
    Proxy proxy = Proxy<T, N>{this};
    proxy.addIndex(index);
    return proxy;
}


/*!
@return Итератор на начало диапазона элементов
*/
template <typename T, T Default, size_t N>
typename Matrix<T, Default, N>::Iterator Matrix<T, Default, N>::begin() {
    return m_data.begin();
}


/*!
@return Итератор на конец диапазона элементов
*/
template <typename T, T Default, size_t N>
typename Matrix<T, Default, N>::Iterator Matrix<T, Default, N>::end() {
    return m_data.end();
}

/// @brief сокращение для двумерной матрицы с нулевым значением по умолчанию
template <typename T, T Default = 0> using Matrix2D = Matrix<T, Default, 2>;
/// @brief сокращение для трехмерной матрицы с нулевым значением по умолчанию
template <typename T, T Default = 0> using Matrix3D = Matrix<T, Default, 3>;
