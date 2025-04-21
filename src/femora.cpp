#include "vector2d.h"
#include <iostream>
#include "config.h"
int main(){
    Femora::vector2d<real> v1(1,2), v2(4.5, 7.7), v3;
    std::cout << v1 + v2 << std::endl;
    std::cout << v1 + v3 << std::endl;
    std::cout << v1 * v2 << std::endl;
    std::cout << v1 * v3 << std::endl;
    std::cout << v1.cross(v2) << std::endl;
    std::cout << v2 * 1.45 << std::endl;
    std::cout << v2 / 72.981 << std::endl;
    std::cout << "sizeof(real) = " << sizeof(real) << " bytes\n";


    return 0;
}