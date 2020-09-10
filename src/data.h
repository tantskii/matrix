/*!
@file
@brief Заголовочный файл с описанием и реализацией класса,
 осуществляющего хранение данных разреженной матрицы
*/

#pragma once

#include "data_helpers.h"

#include <list>
#include <map>
#include <string>

/*!
@brief Класс, который отвечает за хранение данных
@details
@tparam T тип хранимых данных
@tparam N n-мерность матрицы
*/
template <typename T, size_t N>
class Data {
public:
    /// Набор возможных результатов  поиска
    enum class FindStatus {
        FOUND,    ///< Указывает, что объект был найден
        NOT_FOUND ///< Указывает, что объект не был найден
    };
    
    /// @brief тип ключа, представляет из себя std::tuple из N индексов типа size_t
    using Key      = decltype(key_type (std::make_index_sequence<N>{}));
    
    /// @brief тип хранимого элемента, представляет из себя std::tuple из N индексов типа size_t и последющим значением типа T
    using Element  = decltype(elem_type(std::make_index_sequence<N>{}, T{}));
    
    /// @brief сокращение для итератора в std::list<Element>
    using It       = typename std::list<Element>::iterator;
    
    /// @brief сокращение для итератора в std::map<Key, It>
    using MapIt    = typename std::map<Key, It>::const_iterator;
    
    void erase(const Key& key);                               ///< Удаляет элемент по ключу
    void insert(const Key& key, const T& elem);               ///< Добавляет элемент
    bool contains(const Key& key) const;                      ///< Проверяет, есть ли элемент по переданному ключу
    std::pair<FindStatus, T> find(const Key& key) const;      ///< Находит элемент по ключу
    size_t size() const;                                      ///< Возвращает количесвто хранимых элементов
    It begin();
    It end();
    Key makeKey(const Indexes<N>& indexes) const;                ///< Создает ключ
    Element makeElement(const Key& key, const T& elem) const; ///< Создает элемент
private:
    void erase(MapIt it);          ///< Удаление по переданному итератору
    bool contains(MapIt it) const; ///< Проверяет, есть ли элемент по переданному итератору
    
    std::list<Element> m_data; ///< Хранит последовательность из данных типа Element
    std::map<Key, It> m_map;   ///< Ключом явлется Key, а значение это итератор на элемент в m_data
};


/*!
Удаляет элемент по переданному ключу
@param key Ключ удаляемого элемент
@throw std::runtime_error В случае удаления по несуществующему ключу
*/
template <typename T, size_t N>
void Data<T, N>::erase(const Key& key) {
    MapIt it = m_map.find(key);
    if (!contains(it)) {
        throw std::runtime_error("Try to erase element by key which was not created");
    }
    erase(it);
}


/*!
Удаляет элемент по переданному итератору
@param it Итератор на m_map
*/
template <typename T, size_t N>
void Data<T, N>::erase(MapIt it) {
    m_data.erase(it->second);
    m_map.erase(it);
}


/*!
Добавляет элемент. В случае, когда элемент с таким ключом существует, предыдущее значение удаляется.
@param key Ключ проверяемого элемент
@param val Хранимое значение
*/
template <typename T, size_t N>
void Data<T, N>::insert(const Key& key, const T& val) {
    MapIt it = m_map.find(key);
    if (contains(it)) {
        erase(it);
    }
    m_data.push_back(makeElement(key, val));
    m_map.emplace(key, std::prev(m_data.end()));
}


/*!
Проверяет, существует ли элемент по переданному ключу
@param key Ключ проверяемого элемента
@return true если элемент по переданному ключу существует, false -- если нет
*/
template <typename T, size_t N>
bool Data<T, N>::contains(const Key& key) const {
    MapIt it = m_map.find(key);
    return contains(it);
}


/*!
Проверяет, существует ли элемент по переданному итератору
@param it Итератор на m_map
@return true если элемент по переданному итератору существует, false -- если нет
*/
template <typename T, size_t N>
bool Data<T, N>::contains(MapIt it) const {
    return it != m_map.end();
}


/*!
Осуществляет поиск элемента по ключу
@param key Ключ искомого элемента
@return Если элемента нет, то пару FindStatus::NOT_FOUND и значение типа T по умолчанию. В противном случае
 возвращается пара FindStatus::FOUND и значение типа T
*/
template <typename T, size_t N>
std::pair<typename Data<T, N>::FindStatus, T> Data<T, N>::find(const Key& key) const {
    MapIt it = m_map.find(key);
    if (!contains(it)) {
        return {FindStatus::NOT_FOUND, T{}};
    }
    return {FindStatus::FOUND, std::get<N>(*(it->second))};
}


/*!
Создает ключ по набору индексов
@param indexes Набор индексов
@return Ключ
*/
template <typename T, size_t N>
typename Data<T, N>::Key Data<T, N>::makeKey(const Indexes<N>& indexes) const {
    if (indexes.size() != N) {
        std::string error_message = "Indexes size must be "  + \
                                    std::to_string(N) + " not " + \
                                    std::to_string(indexes.size());
        throw std::runtime_error(error_message);
    }
    return makeKeyImpl(indexes, std::make_index_sequence<N>{});
}


/*!
Создает элемент
@param key Ключ
@return Хранимое значение типа Eleement
*/
template <typename T, size_t N>
typename Data<T, N>::Element Data<T, N>::makeElement(const Key& key, const T& elem) const {
    return makeElemImpl(key, std::make_index_sequence<N>{}, elem);
}


/*!
Возвращает количество хранимых элементов
@return количество хранимых элементов
*/
template <typename T, size_t N>
size_t Data<T, N>::size() const {
    return m_data.size();
}


/*!
Возвращает итератор на начало диапазона
@return итератор на начало диапазона
*/
template <typename T, size_t N>
typename Data<T, N>::It Data<T, N>::begin() {
    return m_data.begin();
}


/*!
Возвращает итератор на конец диапазона
@return итератор на конец диапазона
*/
template <typename T, size_t N>
typename Data<T, N>::It Data<T, N>::end() {
    return m_data.end();
}
