#ifndef MESH_H 
#define MESH_H
#include <vector>
#include <string>
#include "vector2d.h"
#include "config.h"
namespace Femora{

enum MeshReverse{
    ReverseNodes,
    ReversePhysicalNames,
    ReverseElements
};
    
struct PhysicalName{


    int dimension;
    int tag;
    std::string name; 
    //emplace_back对于结构体，要在有构造函数的情况下才能使用。

    PhysicalName(int dim, int t, const std::string& n): dimension(dim), tag(t), name(n){}
};


struct Element{
    int id;
    int type;
    int tagNum;
    //tags内容：
    //第一个：物理组tag，
    //第二个：生成来源：生成时来源于哪个边/面，暂时没用
    
    std::vector<int> tags;
    
    std::vector<int> nodesList;
    Element(
        const int i,
        const int tp,
        const int tgnm,
        const std::vector<int>& inTag,
        const std::vector<int>& inNodesList
    ) : id(i),
    type(tp),
    tagNum(tgnm),
    tags(inTag),
    nodesList(inNodesList)
    {}
};

class Mesh{
private:
    std::vector<vector2d<real>> nodes;
    //NodeNumberByVectorIndex的索引是Nodes的索引,值代表gmsh中的序号
    //统一语言：Number代表gmsh里的序号，Index代表vector的索引
    std::vector<int> nodeNumberByVectorIndex;

    std::vector<PhysicalName> physicalNames;

    std::vector<Element> elements;



public:
void reserveObject(MeshReverse target, size_t number);
    void addNode(int number, real x, real y);
    void printNodes();
    int getNodeNumberByVectorIndex(size_t index);

    void addPhysicalName(const int dim, const int tag, const std::string& name);
    void printPhysicalNames();

    void addElement(
        const int i,
        const int tp,
        const int tgnm,
        const std::vector<int>& inTag,
        const std::vector<int>& inNodesList
    );
    void printElements();

    //根据物理标签获取单元的ID。
    std::vector<int> getElementIdsByPhysicalTag(int tag);
    //根据物理标签获取单元vector的索引。
    std::vector<size_t> getElementIndexesByPhysicalTag(int tag);

    Element getElementByIndex(size_t index){
        return elements[index];
    }
    
    
    



};





}


#endif //MESH_H