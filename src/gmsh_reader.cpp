#include "gmsh_reader.h"
#include <fstream>
#include <stdexcept>
#include <sstream> 



bool Femora::GmeshReader::load(const std::string& fileName, Mesh& mesh){
    std::ifstream inGmsh(fileName);
    if(!inGmsh.is_open()){
        
        throw std::runtime_error("Failed to Open the Gmsh file: " + fileName);
    }
    
    parsePhysicalNames(inGmsh, mesh);
    parseNodes(inGmsh, mesh);
    parseElements(inGmsh, mesh);
    
    



    

    return true;
}

void Femora::GmeshReader::parsePhysicalNames(std::ifstream& inGmsh, Mesh& mesh){
    std::string line;
    //获得PhysicalNames的个数
    int physicalNameNum;
    while(std::getline(inGmsh, line)){
        if(line == "$PhysicalNames"){
            std::getline(inGmsh, line);
            //"$读取PhysicalNames"的下一行代表点的个数,获得点的个数
            physicalNameNum = std::stoi(line);
            
            break;
        }
    }
    mesh.reserveObject(MeshReverse::ReversePhysicalNames, physicalNameNum);//预先分配内存
    int inDimension, inTag;
    std::string inName;
    //开始读取点
    for(int i = 0; i < physicalNameNum; i++){
        std::getline(inGmsh, line);
        std::istringstream iss(line);

        iss >> inDimension >> inTag;
        std::getline(iss, inName);
        if(inName.empty()){
            throw std::runtime_error("Wrong format of the name of a PhysicalName : empty Name" );
        }
        while(inName.front() == ' '){
            //删除开头空格，从 inName 的索引 1 开始，取长度为 inName.size() - 1 的子串
            inName = inName.substr(1, inName.size() - 1);
        }
        if(inName.size() > 2 && inName.front() == '"' && inName.back() == '"'){
            //从 inName 的索引 1 开始，取长度为 inName.size() - 2 的子串
            inName = inName.substr(1, inName.size() - 2);
        }
        else{
            throw std::runtime_error("Wrong format of the name of a PhysicalName :" + inName);
        }
        mesh.addPhysicalName(inDimension, inTag, inName);
        
    }


    //查看是否到$EndPhysicalNames行
    std::getline(inGmsh, line);
    if(line != "$EndPhysicalNames"){
        
        throw std::runtime_error("Where it should be $EndPhysicalNames is: " + line);

    }
}
void Femora::GmeshReader::parseNodes(std::ifstream& inGmsh, Mesh& mesh){
    std::string line;
    //获得点的个数
    int nodeNum;
    while(std::getline(inGmsh, line)){
        if(line == "$Nodes"){
            std::getline(inGmsh, line);
            //"$Nodes"的下一行代表点的个数,获得点的个数
            nodeNum = std::stoi(line);
            
            break;
        }
    }

    
    mesh.reserveObject(MeshReverse::ReverseNodes, nodeNum);//预先分配内存
    int number;
    real nodeX, nodeY;
    //开始读取点
    for(int i = 0; i < nodeNum; i++){
        std::getline(inGmsh, line);
        std::istringstream iss(line);

        iss >> number >> nodeX >> nodeY;
        mesh.addNode(number, nodeX, nodeY);
        
    }


    //查看是否到$EndNodes行
    std::getline(inGmsh, line);
    if(line != "$EndNodes"){
        
        throw std::runtime_error("Where it should be $EndNodes is: " + line);

    }

}
void Femora::GmeshReader::parseElements(std::ifstream& inGmsh, Mesh& mesh){
    std::string line;
    ///////////////////////////////////////////
    //获得单元的个数
    int elementNum;
    while(std::getline(inGmsh, line)){
        if(line == "$Elements"){
            std::getline(inGmsh, line);
            //"$Elements"的下一行代表点的个数,获得个数
            elementNum = std::stoi(line);
            
            break;
        }
    }

    
    mesh.reserveObject(MeshReverse::ReverseElements, elementNum);//预先分配内存
    int id,type,tagNum;

    
    //开始读取点
    for(int i = 0; i < elementNum; i++){
        std::vector<int> tags;
        std::vector<int> nodesList;
        std::getline(inGmsh, line);
        std::istringstream iss(line);

        iss >> id >> type >> tagNum;
        int tag;
        for(int i = 0; i < tagNum; i++){
            iss >> tag;
            tags.push_back(tag);
        }
        int nodeNum;
        switch(type){
            case 1 : nodeNum = 2; break;
            case 2 : nodeNum = 3; break;
            case 3 : nodeNum = 4; break;
            case 4 : nodeNum = 4; break;
            case 5 : nodeNum = 8; break;
            default: 
                throw std::runtime_error("Unsupported element type: " + std::to_string(type));
        }
        int node;
        for(int i = 0; i < nodeNum; i++){
            iss >> node;
            nodesList.push_back(node);
        }
        mesh.addElement(id, type, tagNum, tags, nodesList);
        
    }


    //查看是否到$EndElements行
    std::getline(inGmsh, line);
    if(line != "$EndElements"){
        
        throw std::runtime_error("Where it should be $EndElements is: " + line);

    }

}