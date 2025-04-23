#ifndef GMSH_READER_H
#define GMSH_READER_H

#include <vector>
#include "vector2d.h"
#include <string>
#include "mesh.h"


namespace Femora{
class GmeshReader{
public:
    bool load(const std::string& fileName, Mesh& mesh);
private:
    void parsePhysicalNames(std::ifstream& inGmsh, Mesh& mesh);
    void parseNodes(std::ifstream& inGmsh, Mesh& mesh);
    void parseElements(std::ifstream& inGmsh, Mesh& mesh);

};


}

#endif //GMSH_READER_H