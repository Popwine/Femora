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
    real domainSizeX_;
    real domainSizeY_;

    std::vector<std::vector<DataType>> data_;
public:
    size_t getWidth() const{return width_;};
    size_t getHeight() const{return height_;};
    real getDx() const{return dx_;};
    real getDomainSizeX() const{return domainSizeX_;};
    real getDomainSizeY() const{return domainSizeY_;};
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
    domainSizeX_ = (w - 1) * dx;
    domainSizeY_ = (h - 1) * dx;
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
    //浮点数索引
    real x_idx_float = x / dx_;
    real y_idx_float = y / dx_;

    //整数索引
    int j = std::floor(x_idx_float);
    int i = std::floor(y_idx_float);
    if (j < 0 || i < 0 || j + 1 >= static_cast<int>(width_) || i + 1 >= static_cast<int>(height_)){
        return DataType{};
    }
    else{
        real tx = x_idx_float - j;
        real ty = y_idx_float - i;

        // 获取单元格四个角点的值
        const DataType& q11 = data_[i][j];       // 左下
        const DataType& q21 = data_[i][j+1];     // 右下
        const DataType& q12 = data_[i+1][j];     // 左上
        const DataType& q22 = data_[i+1][j+1];   // 右上

        // 在 x 方向进行两次线性插值
        DataType f_xy1 = (1.0 - tx) * q11 + tx * q21;
        DataType f_xy2 = (1.0 - tx) * q12 + tx * q22;

        // 在 y 方向对上述结果进行线性插值
        DataType result = (1.0 - ty) * f_xy1 + ty * f_xy2;

        return result;
    }


}

template<typename DataType>
UniformField<DataType> advect(const UniformField<DataType>& field, 
    const UniformField<vector2d<DataType>>& vectorField, 
    const real timeStepLength){
    if(field.getHeight() != vectorField.getHeight()
        || field.getWidth() != vectorField.getWidth()
        || field.getDx() != vectorField.getDx()
    )
    {
        throw std::runtime_error("The parameters of field don't match that of vectorField.");
    }
   
    UniformField<DataType> advectedField(field.getWidth(), field.getHeight(), field.getDx(), DataType{});
    for(size_t i = 0; i < field.getHeight(); i++){
        for(size_t j = 0; j < field.getWidth(); j++){
            real backtracedX = j * field.getDx() - timeStepLength * vectorField.getValue(i, j).x;
            real backtracedY = i * field.getDx() - timeStepLength * vectorField.getValue(i, j).y;
            if(backtracedX < 0) backtracedX = 0;
            if(backtracedY < 0) backtracedY = 0;
            if(backtracedX > field.getDomainSizeX()) backtracedX = field.getDomainSizeX();
            if(backtracedY > field.getDomainSizeY()) backtracedY = field.getDomainSizeY();
            DataType backtracedValue = field.interpolateValue(backtracedX, backtracedY);
            advectedField.setValue(i, j, backtracedValue);
        }
    }
    return advectedField;
    
}


}





#endif //FIELD_H