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



template<typename DataType>
class UniformField{
//正方形网格流场
private:
    size_t width_;//宽度方向的网格数
    size_t height_;//高度方向的网格数
    real dx_;//正方形网格大小
    std::vector<std::vector<DataType>> data_;
public:
    size_t getWidth(){return width_;};
    size_t getHeight(){return height_;};
    real getDx(){return dx_;};
    UniformField(const size_t w, const size_t h, const real dx, const DataType& Value);
    
    const DataType& getValue(size_t i, size_t j);


    void setValue(size_t i, size_t j, const DataType& Value);
    void setValueAll(const DataType& Value);
    void print();
    
    UniformField partialDerivativeX() const;
    UniformField partialDerivativeY() const;
    UniformField laplace() const;
    
};

template<typename DataType>
UniformField<DataType>::UniformField(
    const size_t w, 
    const size_t h, 
    const real dx, 
    const DataType& Value)
    :
    width_(w), 
    height_(h),
    dx_(dx)
    {
    //规定行的个数（height_）
    data_.resize(h);
    for(auto& row : data_){
        row.resize(w);
        std::fill(row.begin(),row.end(),Value);
    }
    


}
template<typename DataType>
const DataType& UniformField<DataType>::getValue(size_t i, size_t j){
    //get field value from indexes
    return data_[i][j];
}

template<typename DataType>
void UniformField<DataType>::setValue(size_t i, size_t j, const DataType& Value){
    //set field value from indexes
    data_[i][j] = Value;
}

template<typename DataType>
void UniformField<DataType>::print(){
    for(const auto& row : data_){
        for(const auto& value : row){
            std::cout << value << " " ;
        }
        std::cout << std::endl;
    }
}



/**
 * @brief set all the value
 * @param Value specific value
 */
template<typename DataType>
void UniformField<DataType>::setValueAll(const DataType& Value){
    for(auto& row : data_){
        std::fill(row.begin(), row.end(), Value);
    }
}

template<typename DataType>
UniformField<DataType> UniformField<DataType>::partialDerivativeX() const{
    DataType defaultValue{};
   
    UniformField<DataType> result(width_, height_, dx_, defaultValue);
    for(size_t i = 1; i < height_ - 1; i++){
        for(size_t j = 1; j < width_ - 1; j++){
            
            result.setValue(i, j, (data_[i][j+1] - data_[i][j-1]) / (2.0 * dx_));
            
        }
    }
    return result;
}

template<typename DataType>
UniformField<DataType> UniformField<DataType>::partialDerivativeY() const{
    DataType defaultValue{};
   
    UniformField<DataType> result(width_, height_, dx_, defaultValue);
    for(size_t i = 1; i < height_ - 1; i++){
        for(size_t j = 1; j < width_ - 1; j++){
            
            result.setValue(i, j, (data_[i+1][j] - data_[i-1][j]) / (2.0 * dx_));
        }
    }
    return result;
}

template<typename DataType>
UniformField<DataType> UniformField<DataType>::laplace() const{
    DataType defaultValue{};
   
    UniformField<DataType> result(width_, height_, dx_, defaultValue);
    for(size_t i = 1; i < height_ - 1; i++){
        for(size_t j = 1; j < width_ - 1; j++){
            
            result.setValue(i, j, (
                data_[i+1][j] + data_[i-1][j] + data_[i][j+1] + data_[i][j-1]
                - 4 * data_[i][j]
            ) / (dx_ * dx_));
        }
    }
    return result;
}




}





#endif //FIELD_H