#ifndef MESH_H 
#define MESH_H
#include <vector>
#include "vector2d.h"
#include "config.h"
namespace Femora{


class Mesh{
private:
    std::vector<vector2d<real>> vertices;
    //NodeNumberByVectorIndex的索引是vertices的索引,值代表gmsh中的序号
    std::vector<int> NodeNumberByVectorIndex;
    

public:
    void addVertex(int number, real x, real y);
    void printVertices();
    int getNodeNumberByVectorIndex(size_t index);

    
    



};




}


#endif //MESH_H