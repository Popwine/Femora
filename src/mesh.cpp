#include "mesh.h"
#include <iterator>
#include <iostream>
namespace Femora{

void Mesh::addVertex(int number, real x, real y){
    NodeNumberByVectorIndex.emplace_back(number);
    vertices.emplace_back(x, y);
}

void Mesh::printVertices(){
    for(const auto& v : vertices){
        std::cout << v << std::endl;
    }
}
int Mesh::getNodeNumberByVectorIndex(size_t index){
    return NodeNumberByVectorIndex[index];
}

}