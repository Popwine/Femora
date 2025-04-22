#include "gmsh_reader.h"
#include <fstream>
#include <stdexcept>
#include <sstream> 


bool Femora::GmeshReader::load(const std::string& fileName, Mesh& mesh){
    std::ifstream inGmsh(fileName);
    std::string line;
    int nodeNum;
    if(!inGmsh.is_open()){
        
        throw std::runtime_error("Failed to Open the Gmsh file: " + fileName);
    }
    //获得点的个数
    while(std::getline(inGmsh, line)){
        if(line == "$Nodes"){
            std::getline(inGmsh, line);
            //"$Nodes"的下一行代表点的个数,获得点的个数
            nodeNum = std::stoi(line);
            
            break;
        }
    }

    
    mesh.reserveVertices(nodeNum);//预先分配内存
    int number;
    real nodeX, nodeY;
    //开始读取点
    for(int i = 0; i < nodeNum; i++){
        std::getline(inGmsh, line);
        std::istringstream iss(line);

        iss >> number >> nodeX >> nodeY;
        mesh.addVertex(number, nodeX, nodeY);
        
    }


    //查看是否到$EndNodes行
    std::getline(inGmsh, line);
    if(line.rfind("$EndNodes") == std::string::npos){
        
        throw std::runtime_error("Where it should be $EndNodes is: " + line);

    }

    return true;
}