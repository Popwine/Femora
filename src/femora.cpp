#include "vector2d.h"
#include <iostream>
#include "config.h"
#include "mesh.h"
#include "gmsh_reader.h"
#include "field.h"

#include "matrix_csr.h"


// 辅助函数：打印 vector
template<typename T>
void print_vector(const std::vector<T>& vec, const std::string& name = "Vector") {
    std::cout << name << " (" << vec.size() << "): [ ";
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << "]" << std::endl;
}

void test_mesh_and_field(){
    Femora::Mesh mesh;
    Femora::GmeshReader reader;
    reader.load("gmsh_files/rect_3x4.msh", mesh);
    mesh.printNodes();

    mesh.printPhysicalNames();
    mesh.printElements();
    std::vector<int> findedElementNumber;
    findedElementNumber = mesh.getElementIdsByPhysicalTag(5);
    for(const auto& n : findedElementNumber){
        std::cout << n << " ";
    }
    std::cout << std::endl;

    const std::vector<size_t>& findedElementIndexes = mesh.getElementIndexesByPhysicalTag(5);
    for(const auto& n : findedElementIndexes){
        std::cout << mesh.getElementByIndex(n).id << " ";
    }
    std::cout << std::endl;
    
    Femora::Field<real> speed_x(mesh);
    real v1 = 5.5;
    real v2 = 3.1;
    speed_x.initialize(v1);
    
    for(const auto& n : findedElementIndexes){
        std::cout << speed_x.getValue(n) << std::endl;
    }
    speed_x.initialize(v2);

    for(const auto& n : findedElementIndexes){
        std::cout << speed_x.getValue(n) << std::endl;
    }

}

int testCsr(){
    // --- 1. 手动创建一个已知的稀疏矩阵 A (CSR 格式) ---
    // 假设我们要表示以下 4x4 矩阵 A:
    //   [ 5  0 -1  0 ]
    //   [ 0  2  0  3 ]
    //   [-1  0  4  0 ]
    //   [ 0  3  0  6 ]

    Femora::SparseMatrixCSR A;
    A.num_rows = 4;
    A.num_cols = 4;

    // 非零元素值 (按行优先)
    A.values = { 5.0, -1.0,    // Row 0
                 2.0,  3.0,    // Row 1
                -1.0,  4.0,    // Row 2
                 3.0,  6.0 };  // Row 3
                 // 注意: 类型应为 real, 这里用 5.0 显式表示浮点数

    // 非零元素对应的列索引
    A.col_indices = { 0, 2,        // Row 0: A[0][0], A[0][2]
                      1, 3,        // Row 1: A[1][1], A[1][3]
                      0, 2,        // Row 2: A[2][0], A[2][2]
                      1, 3 };      // Row 3: A[3][1], A[3][3]
                      // 注意: 索引从 0 开始

    // 行指针 (row_ptr[i] 是第 i 行第一个非零元在 values/col_indices 中的起始位置)
    // 大小为 num_rows + 1
    A.row_ptr = { 0,        // Row 0 starts at index 0
                  2,        // Row 1 starts at index 2
                  4,        // Row 2 starts at index 4
                  6,        // Row 3 starts at index 6
                  8 };      // End marker (total number of non-zero elements)

    // --- 2. 创建一个已知的稠密向量 x ---
    std::vector<real> x = {1.0, 2.0, 3.0, 4.0}; // 大小需等于 A.num_cols
    print_vector(x, "Input vector x");

    // --- (可选) 检查输入向量大小是否匹配 ---
    assert(x.size() == A.num_cols && "Input vector x size must match matrix number of columns!");

    // --- 3. 调用你的 spmv_csr 函数计算 y = Ax ---
    std::vector<real> y;
    try {
        y = Femora::spmv_csr(A, x);
    } catch (const std::exception& e) {
        std::cerr << "Error during SpMV calculation: " << e.what() << std::endl;
        return 1; // 异常退出
    }

    // --- (可选) 检查输出向量大小是否匹配 ---
     assert(y.size() == A.num_rows && "Output vector y size must match matrix number of rows!");

    // --- 4. 打印计算结果 y ---
    print_vector(y, "Result vector y = Ax");

    // --- 5. 手动计算正确结果进行对比 ---
    // y[0] = A[0][0]*x[0] + A[0][2]*x[2] = 5*1 + (-1)*3 = 2
    // y[1] = A[1][1]*x[1] + A[1][3]*x[3] = 2*2 + 3*4 = 16
    // y[2] = A[2][0]*x[0] + A[2][2]*x[2] = (-1)*1 + 4*3 = 11
    // y[3] = A[3][1]*x[1] + A[3][3]*x[3] = 3*2 + 6*4 = 30
    std::vector<real> expected_y = {2.0, 16.0, 11.0, 30.0};
    print_vector(expected_y, "Expected vector y");

    // --- (可选) 简单比较结果 ---
    bool match = true;
    if (y.size() == expected_y.size()) {
        for (size_t i = 0; i < y.size(); ++i) {
            // 使用一个小的容差比较浮点数
            if (std::abs(y[i] - expected_y[i]) > 1e-6) {
                match = false;
                break;
            }
        }
    } else {
        match = false;
    }

    if (match) {
        std::cout << "Result matches expected value!" << std::endl;
    } else {
        std::cout << "Result does NOT match expected value!" << std::endl;
    }

    return 0;
}

void test_UniformField(){
    Femora::UniformField<real> field(5,5,0.1,9.2);
    
    field.setValue(2,2,7.5);
    field.setValue(3,1,9.5);
    field.setValue(1,4,9.23);
    field.setValue(4,4,9.01);
    field.setValue(1,2,9.36);
    field.setValue(3,1,8.8);
    std::cout << "field: " << std::endl;
    field.print();

    auto partialDerivativeX = field.partialDerivativeX();
    std::cout << "field partial x: " << std::endl;
    partialDerivativeX.print();

    auto partialDerivativeY = field.partialDerivativeY();
    std::cout << "field partial Y: " << std::endl;
    partialDerivativeY.print();

    auto laplace = field.laplace();
    std::cout << "field laplace: " << std::endl;
    laplace.print();

    Femora::vector2d<real> vec(1.2, 2.3);
    Femora::UniformField<Femora::vector2d<real>> vectorField(5, 5, 0.1, vec);
    vectorField.setValue(1,1,Femora::vector2d<real>(1.5,1.9));
    vectorField.setValue(0,3,Femora::vector2d<real>(0.8,1.6));
    vectorField.setValue(1,3,Femora::vector2d<real>(1.1,0.7));
    vectorField.setValue(2,2,Femora::vector2d<real>(1.9,1.3));
    vectorField.setValue(3,0,Femora::vector2d<real>(0.7,1.8));
    vectorField.setValue(4,1,Femora::vector2d<real>(1.1,0.5));
    vectorField.setValue(2,4,Femora::vector2d<real>(1.3,0.6));
    std::cout << "vectorField: " << std::endl;
    vectorField.print();

    auto divegence = Femora::divergence(vectorField);
    std::cout << "vectorField divergence: " << std::endl;
    divegence.print();

    auto gradient = field.gradient();
    std::cout << "field gradient: " << std::endl;
    gradient.print();

    auto curl_2d = Femora::curl_2d(vectorField);
    std::cout << "vectorField curl_2d: " << std::endl;
    curl_2d.print();
    std::cout << "interpolate value 0.15, 0.25: " << std::endl;
    std::cout << field.interpolateValue(0.19, 0.29) << std::endl;

    Femora::UniformField<real> advected_field = Femora::advect(field, vectorField, 0.001);
    std::cout << "advected_field: " << std::endl;
    advected_field.print();


}
int main(){
    std::cout << "Femora is running." << std::endl;


    //test_mesh_and_field();
    //testCsr();

    test_UniformField();

    std::cout << "Femora is exiting." << std::endl;
    return 0;
}