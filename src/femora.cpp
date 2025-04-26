#include "vector2d.h"
#include <iostream>
#include "config.h"
#include "mesh.h"
#include "gmsh_reader.h"
#include "field.h"
int main(){
    std::cout << "Femora is running." << std::endl;
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





    std::cout << "Femora is exiting." << std::endl;
    return 0;
}