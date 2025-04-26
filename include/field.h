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
    void initialize(DataType& value);
    explicit Field(const Mesh& m);
    Field(const Mesh& m, DataType value);
    ~Field();
    void Clear();
    void setValue(size_t Idx, DataType value);
    DataType getValue(size_t Idx) const;



};

template<typename DataType>
Field<DataType>::Field(const Mesh& m) : mesh(m)
{
    
}

template<typename DataType>
Field<DataType>::Field(const Mesh& m, DataType value) : mesh(m)
{
    initialize(value);
}

template<typename DataType>
Field<DataType>::~Field()
{
}

template<typename DataType>
void Field<DataType>::initialize(DataType& value){
    
    data_.assign(mesh.getNumElements(), value);

}

template<typename DataType>
void Field<DataType>::Clear()
{
    data_.clear();
}

template<typename DataType>
void Field<DataType>::setValue(size_t Idx, DataType value)
{
    if(Idx < data_.size() ){
        data_[Idx] = value;
    }
    else{
        throw std::runtime_error("Index out of range.");
    }
    
}

template<typename DataType>
DataType Field<DataType>::getValue(size_t Idx)const{
    return data_[Idx];
}


}





#endif //FIELD_H