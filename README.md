# Femora

**Femora** 是一个可扩展的 2D 流体/有限元仿真框架，采用模块化设计，支持网格读取、基础数学运算和通用仿真算法架构。

## 特性

- **模块化架构**：MathCore、MeshIO、Solver、IO 分离，易扩展
- **支持 Gmsh 网格**（ASCII `.msh2`）读取，兼容自定义格式
- **基于 C++17**：使用现代 C++ 特性，如 `constexpr`、`std::vector`、`emplace_back` 等
- **精度切换**：通过 `USE_DOUBLE_PRECISION` 宏在 `float`/`double` 之间切换
- **CMake 构建**：支持 MinGW、MSVC、GCC 等多种编译工具链
- **Git 管理**：GitHub 仓库管理、CI/CD 支持（可扩展）

## 目录结构

```
Femora/
├── CMakeLists.txt       # 顶层构建脚本
├── README.md            # 项目说明
├── include/             # 公共头文件
│   ├── Femora/          # 包含命名空间 Femora
│   │   ├── config.h     # 精度切换定义
│   │   ├── vector2d.h   # 二维向量模板类
│   │   ├── mesh.h       # Mesh 核心类声明
│   │   └── gmsh_reader.h# GmshReader 类声明
├── src/                 # 源码实现
│   ├── vector2d.cpp     # MathCore 模块实现
│   ├── mesh.cpp         # Mesh 类实现
│   ├── gmsh_reader.cpp  # GmshParser 实现
│   └── femora.cpp       # 主程序示例 & 测试
├── gmsh_files/          # 示例 Gmsh 网格文件
│   ├── rect_3x4.geo     # Gmsh Geometry
│   └── rect_3x4.msh     # 对应 Mesh 文件（ASCII msh2）
└── build/               # 构建目录（忽略）
```

## 构建步骤

1. **Clone 仓库**

   ```bash
   git clone git@github.com:Popwine/Femora.git
   cd Femora
   ```

2. **创建构建目录并生成 Makefile**

   - **使用 MinGW（Windows）**

     ```bash
     mkdir build-mingw && cd build-mingw
     cmake .. -G "MinGW Makefiles" -DUSE_DOUBLE_PRECISION=ON
     mingw32-make
     ```

   - **使用 Unix Makefile（Linux/macOS）**

     ```bash
     mkdir build && cd build
     cmake .. -DUSE_DOUBLE_PRECISION=OFF
     make -j4
     ```

3. **运行示例**

   ```bash
   ./femora ../gmsh_files/rect_3x4.msh
   ```

## 使用说明

- **Mesh 类**

  - `addNode(x, y)`：添加节点
  - `addElement({i0, i1, ...}, type)`：添加单元
  - `getNumNodes()`、`getNumElements()`：获取尺寸
  - `clear()`：重置 Mesh

- **GmshReader 类**

  - `load(filename, mesh)`：读取 Gmsh `.msh2` 文件并填充 `Mesh`

- **精度切换**

  - 在 CMake 里设置 `-DUSE_DOUBLE_PRECISION=ON/OFF`，或在 `include/Femora/types.h` 中切换

## 技术栈

- **C++17**：模板编程、`std::vector`、`constexpr` 等
- **CMake ≥3.10**：跨平台构建系统
- **MinGW / GCC / MSVC**：多编译器支持
- **Git**：版本控制、协同开发
- **Gmsh**：网格生成与格式兼容

## 后续功能规划

- **Solver 模块**：实现流体平流、FEM 刚度矩阵等核心算法
- **OpenGL 可视化**：实时渲染网格与仿真结果
- **并行化**：多线程或 GPU 加速核心计算
- **自定义 MeshIO**：支持 VTK、SU2 等多格式读写

---



