#ifndef FIELD_H
#define FIELD_H

#include "mesh.h"
#include "config.h"
#include "vector2d.h"


namespace Femora{
template<typename DataType>
class Field
{
private:
    const Mesh& mesh;
    std::vector<DataType> data_;
public:
    void initialize(const DataType& value);
    explicit Field(const Mesh& m);
    Field(const Mesh& m, const DataType& value);
    ~Field();
    void Clear();
    void setValue(const size_t Idx, const DataType& value);
    const DataType& getValue(const size_t Idx) const;



};

template<typename DataType>
Field<DataType>::Field(const Mesh& m) : mesh(m)
{
    data_.resize(mesh.getNumElements());
}

template<typename DataType>
Field<DataType>::Field(const Mesh& m, const DataType& value) : mesh(m)
{
    initialize(value);
}

template<typename DataType>
Field<DataType>::~Field()
{
}

template<typename DataType>
void Field<DataType>::initialize(const DataType& value){
    
    data_.assign(mesh.getNumElements(), value);

}

template<typename DataType>
void Field<DataType>::Clear()
{
    data_.clear();
}

template<typename DataType>
void Field<DataType>::setValue(const size_t Idx, const DataType& value)
{
    if(Idx < data_.size() ){
        data_[Idx] = value;
    }
    else{
        throw std::runtime_error("Index " + std::to_string(Idx) + " out of range.");
    }
    
}

template<typename DataType>
const DataType& Field<DataType>::getValue(const size_t Idx)const{
    if(Idx < data_.size() ){
        return data_[Idx];
    }
    else{
        throw std::runtime_error("Index " + std::to_string(Idx) + " out of range.");
    }
    
}


}





#endif //FIELD_H