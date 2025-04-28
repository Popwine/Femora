#ifndef MATRIX_CSR_H
#define MATRIX_CSR_H

#include <vector>
#include "config.h"
#include <stdexcept>

namespace Femora
{


// 假设 CSR 矩阵结构体 (你可以自己定义)
struct SparseMatrixCSR {
    size_t num_rows;
    size_t num_cols; // 对于方阵 N x N, num_rows == num_cols
    std::vector<real> values;       // 非零元素值
    std::vector<int> col_indices;  // 非零元素对应的列索引 (通常用 int 或 size_t)
    std::vector<int> row_ptr;      // 行指针 (大小为 num_rows + 1)
};

// 你需要实现的函数
std::vector<real> spmv_csr(const SparseMatrixCSR& A, const std::vector<real>& x);


std::vector<real> spmv_csr(const SparseMatrixCSR& A, const std::vector<real>& x){
    if(x.size() != A.num_cols){
        throw std::runtime_error("The number of A's colums doesn't match x's length.");
    }
    std::vector<real> result;
    for(int i = 0; i < A.num_cols; i++){
        int current_ptr = A.row_ptr[i];
        real sum = 0.0;
        
        while(true){
            real A_value = A.values[A.col_indices[current_ptr]];
            real x_value = x[A.col_indices[current_ptr]];
            sum += A_value * x_value;
            current_ptr++;
            if(current_ptr >= A.col_indices.size() || current_ptr == A.row_ptr[i+1]){

                break;
            }
        }
        result.push_back(sum);
    }

    return result;
}

}
#endif //MATRIX_CSR_H