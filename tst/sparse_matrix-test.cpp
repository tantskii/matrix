#include "sparse_matrix.h"

#include "gtest/gtest.h"

#include <sstream>


TEST(MatrixTest, DefaultSize) {
    Matrix<int, -1, 2> matrix;
    
    ASSERT_EQ(matrix.size(), 0);
}


TEST(MatrixTest, DefaultValue) {
    Matrix<int, -1, 2> matrix;
    
    auto a = matrix[0][0];
    
    ASSERT_TRUE(a == -1);
    ASSERT_EQ(matrix.size(), 0);
}


TEST(MatrixTest, WriteElem) {
    Matrix<int, -1, 2> matrix;
    
    matrix[100][100] = 314;
    
    ASSERT_TRUE(matrix[100][100] == 314);
    ASSERT_EQ(matrix.size(), 1);
}


TEST(MatrixTest, RemoveElem) {
    Matrix<int, -1, 2> matrix;
    
    matrix[100][100] = 314;
    matrix[100][100] = -1;
    
    ASSERT_TRUE(matrix[100][100] == -1);
    ASSERT_EQ(matrix.size(), 0);
}


TEST(MatrixTest, Iteration) {
    Matrix<int, -1, 2> matrix;
    std::ostringstream os;
    
    matrix[100][100] = 314;
    for(const auto& [x, y, v]: matrix) {
        os << x << y << v << std::endl;
    }
    
    ASSERT_EQ(os.str(), "100100314\n");
}


TEST(MatrixTest, OperatorEq) {
    Matrix<int, -1, 2> matrix;
    
    ((matrix[100][100] = 314) = 0) = 217;
    
    ASSERT_TRUE(matrix[100][100] == 217);
    ASSERT_EQ(matrix.size(), 1);
}


TEST(MatrixTest, IncorrectIndexes) {
    Matrix<int, -1, 2> matrix;
    
    ASSERT_THROW(matrix[100][100][100] = 10, std::runtime_error);
}




