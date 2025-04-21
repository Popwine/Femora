#include "vector2d.h"
#include <iostream>
#include "config.h"
#include "mesh.h"
#include "gmsh_reader.h"
int main(){
    std::cout << "Femora is running." << std::endl;
    Femora::Mesh mesh;
    Femora::GmeshReader reader;
    reader.load("gmsh_files/rect_3x4.msh", mesh);
    mesh.printVertices();

    std::cout << "Femora is exiting." << std::endl;
    return 0;
}