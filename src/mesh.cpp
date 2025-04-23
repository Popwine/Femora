#include "mesh.h"
#include <iterator>
#include <iostream>
namespace Femora{

void Mesh::addNode(int number, real x, real y){
    nodeNumberByVectorIndex.emplace_back(number);
    nodes.emplace_back(x, y);
}

void Mesh::printNodes(){
    for(const auto& v : nodes){
        std::cout << v << std::endl;
    }
}
int Mesh::getNodeNumberByVectorIndex(size_t index){
    return nodeNumberByVectorIndex[index];
}
void Mesh::reserveObject(MeshReverse target, size_t number){

    if(target == MeshReverse::ReverseNodes){
        nodes.reserve(number);
        nodeNumberByVectorIndex.reserve(number);
    }
    else if(target == MeshReverse::ReversePhysicalNames){
        physicalNames.reserve(number);

    }
    else if(target == MeshReverse::ReverseElements){
        physicalNames.reserve(number);

    }
    else{
        throw std::runtime_error("Unexpected reverse target.");
    }
    
}
void Mesh::addPhysicalName(const int dim, const int t, const std::string& n){
    physicalNames.emplace_back(dim, t, n);
}

void Mesh::printPhysicalNames(){
    for(const auto& name : physicalNames){
        std::cout << "dimension: "<< name.dimension ;
        std::cout << " tag: " << name.tag;
        std::cout << " name: " << name.name << std::endl;
    }
}
void Mesh::printElements(){
    for(const auto& e : elements){
        std::cout << "id: "<< e.id ;
        std::cout << " type: " << e.type;
        std::cout << " tag number: " << e.tagNum;
        std::cout << " tags:";
        for(const auto& t : e.tags){
            std::cout << " " << t ;
        }
        std::cout << " nodes:";
        for(const auto& n : e.nodesList){
            std::cout << " " << n;
        }
        std::cout << std::endl;
    }
}

void Mesh::addElement(
    const int i,
    const int tp,
    const int tgnm,
    const std::vector<int>& inTag,
    const std::vector<int>& inNodesList
){
    elements.emplace_back(i, tp, tgnm, inTag, inNodesList);

}

std::vector<int> Mesh::getElementIdsByPhysicalTag(int tag){
    std::vector<int> elementIds;
    for(const auto& e : elements){
        if(e.tags.size() > 0 && e.tags[0] == tag){
            elementIds.push_back(e.id);
        }
    }
    return elementIds;


}

std::vector<size_t> Mesh::getElementIndexesByPhysicalTag(int tag){
    std::vector<size_t> elementIndexes;
    for(size_t i = 0; i < elements.size(); i++){
        if(elements[i].tags.size() > 0 && elements[i].tags[0]== tag){
            elementIndexes.push_back(i);
        }
    }
    return elementIndexes;
}


}
