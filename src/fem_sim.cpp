#include "vector2d.h"
#include <iostream>

int main(){
    vector2d<double> v1(1,2), v2(4.5, 7.7), v3;
    std::cout << v1 + v2 << std::endl;
    std::cout << v1 + v3 << std::endl;
    std::cout << v1 * v2 << std::endl;
    std::cout << v1 * v3 << std::endl;
    std::cout << v1.cross(v2) << std::endl;
    std::cout << v2 * 1.45 << std::endl;
    std::cout << v2 / 0.0 << std::endl;



    return 0;
}