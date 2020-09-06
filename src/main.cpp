#include "sparse_matrix.h"

#include <iostream>

int main() {
    Matrix2D<int> matrix;
    
    for (size_t i = 0; i < 10; ++i) {
        matrix[i][i]     = static_cast<int>(i);
        matrix[i][9 - i] = static_cast<int>(9 - i);
    }
    
    for (size_t x = 1; x <= 8; ++x) {
        for (size_t y = 1; y <= 8; ++y) {
            std::cout << matrix[x][y] << ' ';
        }
        std::cout << std::endl;
    }
    
    std::cout << matrix.size() << std::endl;
    
    for (const auto& [x, y, v] : matrix) {
        std::cout << x << ' ' << y << ' ' << v << std::endl;
    }
    
    return 0;
}
