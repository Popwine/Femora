#ifndef FIELD_H
#define FIELD_H
#include <cmath>
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

/*-----------------------UniformField-----------------------*/

template<typename DataType>
class UniformField{
//正方形网格流场
private:
    size_t width_;//宽度方向的网格数
    size_t height_;//高度方向的网格数
    real dx_;//正方形网格大小
    std::vector<std::vector<DataType>> data_;
public:
    size_t getWidth() const{return width_;};
    size_t getHeight() const{return height_;};
    real getDx() const{return dx_;};
    UniformField(const size_t w, const size_t h, const real dx, const DataType& Value);
    
    const DataType& getValue(size_t i, size_t j) const;


    void setValue(size_t i, size_t j, const DataType& Value);
    void setValueAll(const DataType& Value);
    void print() const;
    
    UniformField partialDerivativeX() const;
    UniformField partialDerivativeY() const;
    UniformField laplace() const;
    UniformField<vector2d<DataType>> gradient() const;
    DataType interpolateValue(real x, real y)const;
    
};
UniformField<real> divergence(const UniformField<vector2d<real>>& vecField);
UniformField<real> curl_2d(const UniformField<vector2d<real>>& vecField);

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
const DataType& UniformField<DataType>::getValue(size_t i, size_t j) const{
    //get field value from indexes
    return data_[i][j];
}

template<typename DataType>
void UniformField<DataType>::setValue(size_t i, size_t j, const DataType& Value){
    //set field value from indexes
    data_[i][j] = Value;
}

template<typename DataType>
void UniformField<DataType>::print() const{
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



template<typename DataType>
UniformField<vector2d<DataType>> UniformField<DataType>::gradient() const{
    UniformField<vector2d<DataType>> result(width_, height_, dx_, vector2d<DataType>{});
    for(size_t i = 1; i < height_ - 1; i++){
        for(size_t j = 1; j < width_ - 1; j++){
            
            result.setValue(i, j, vector2d<DataType>(
                (data_[i][j+1] - data_[i][j-1]) / (2.0 * dx_)
                ,
                (data_[i+1][j] - data_[i-1][j]) / (2.0 * dx_)
            ));

        }
    }

    return result;
}


template<typename DataType>
DataType UniformField<DataType>::interpolateValue(real x, real y)const{
    int i = std::floor(y / dx_);
    int j = std::floor(x / dx_);
    
    if(i < 0 || j < 0 || i >= (int)(height_ - 1) || j >= (int)(width_ - 1)){
        return DataType{};
    }
    else{
        std::cout << data_[i][j] <<", " << data_[i+1][j] <<", " << data_[i][j+1] <<", " << data_[i+1][j+1] << std::endl;
        DataType tx = (x - j * dx_) / dx_;
        DataType ty = (y - i * dx_) / dx_;
        DataType y0 = (1.0 - tx) * data_[i][j] + tx * data_[i][j + 1];
        DataType y1 = (1.0 - tx) * data_[i + 1][j] + tx * data_[i + 1][j + 1];
        DataType result = (1.0 - ty) * y0 + ty * y1;
        std::cout << tx <<", " << ty <<", " << y0 <<", " << y1 << std::endl;
        return result;
    }

}



}





#endif //FIELD_H