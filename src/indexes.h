/*!
@file
@brief Заголовочный класс, содержащий определение набора индексов
*/

#pragma once

#include <array>

/// @brief сокращение для набора индексов
template <size_t N> using Indexes = std::array<size_t, N>;
