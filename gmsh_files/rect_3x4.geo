// rect_3x4_simple.geo — 确保兼容 Gmsh 4.x

// 网格尺寸
nx = 4;
ny = 3;
Lx = 4;
Ly = 3;

// 点编号方式（总共 20 个点）
Point(1) = {0, 0, 0, 1.0};
Point(2) = {1, 0, 0, 1.0};
Point(3) = {2, 0, 0, 1.0};
Point(4) = {3, 0, 0, 1.0};
Point(5) = {4, 0, 0, 1.0};

Point(6) = {0, 1, 0, 1.0};
Point(7) = {1, 1, 0, 1.0};
Point(8) = {2, 1, 0, 1.0};
Point(9) = {3, 1, 0, 1.0};
Point(10) = {4, 1, 0, 1.0};

Point(11) = {0, 2, 0, 1.0};
Point(12) = {1, 2, 0, 1.0};
Point(13) = {2, 2, 0, 1.0};
Point(14) = {3, 2, 0, 1.0};
Point(15) = {4, 2, 0, 1.0};

Point(16) = {0, 3, 0, 1.0};
Point(17) = {1, 3, 0, 1.0};
Point(18) = {2, 3, 0, 1.0};
Point(19) = {3, 3, 0, 1.0};
Point(20) = {4, 3, 0, 1.0};

// 只定义最外边框（让 Gmsh 自动网格化）
Line(1) = {1, 5};
Line(2) = {5, 20};
Line(3) = {20, 16};
Line(4) = {16, 1};

Line Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};

// 物理标签
Physical Line("inlet") = {4};
Physical Line("outlet") = {2};
Physical Line("top") = {3};
Physical Line("bottom") = {1};
Physical Surface("fluid") = {1};
