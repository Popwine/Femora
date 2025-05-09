#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <iostream>
#include <cassert>
#include <stdexcept>

namespace Femora{



template<typename T>
class vector2d{
public:
    T x, y;
    vector2d();
    vector2d(T _x, T _y);
    ~vector2d();
    //因为在 模板类定义内部，Vector2D 就已经等价于 Vector2D<T>，
    //所以可以直接使用 Vector2D 表示该模板类自身，无需写 Vector2D<T>。
    vector2d operator+(const vector2d& rhs) const;
    vector2d operator-(const vector2d& rhs) const;
    T operator*(const vector2d& rhs) const;
    vector2d operator*(T scalar) const;
    vector2d operator/(T scalar) const;

    T cross(const vector2d&rhs) const;

    bool operator==(const vector2d& rhs) const;
    bool operator!=(const vector2d& rhs) const;
    
    //因为左值不为类本身，应声明为友元函数
    //第一个std::ostream&的&是为了支持链式操作
    //std::ostream&不加const是因为输出gchgn其实是在修改ostream对象
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const vector2d<U>& vec);

};

template<typename T>
vector2d<T>::~vector2d() = default;

template<typename T>
vector2d<T>::vector2d(){
    x = T{};
    y = T{};
}

template<typename T>
vector2d<T>::vector2d(T x_, T y_) : x(x_), y(y_) {

}
template<typename T>
vector2d<T> vector2d<T>::operator+(const vector2d<T>& rhs) const{
    return vector2d<T>(x + rhs.x, y + rhs.y);
}

template<typename T>
vector2d<T> vector2d<T>::operator-(const vector2d<T>& rhs) const{
    return vector2d<T>(x - rhs.x, y - rhs.y);
}

template<typename T>
T vector2d<T>::operator*(const vector2d<T>& rhs) const{
    return x * rhs.x + y * rhs.y;
}

template<typename T>
vector2d<T> vector2d<T>::operator*(T scalar) const{
    return vector2d<T>(x * scalar, y * scalar);
}

template<typename T>
vector2d<T> vector2d<T>::operator/(T scalar) const{
    assert(scalar != T(0) && "Division by zero detected during debug!");

    // 在 Release 和 Debug 模式下都进行运行时检查
    // 只有当 T 是算术类型 (int, float, double 等) 时，才进行编译和检查
    if constexpr (std::is_arithmetic<T>::value) {
        if (scalar == T(0)) {
            throw std::runtime_error("Division by zero in vector2d operation.");
        }
    }
    // else {
    //   // 如果 T 不是算术类型 (比如自定义类)，你可能需要不同的处理逻辑
    //   // 或者假设非算术类型的 operator/ 或 T(0) 会自己处理好
    // }

    return vector2d<T>(x / scalar, y / scalar);
}

template<typename T>
T vector2d<T>::cross(const vector2d<T>& rhs) const{
    return x * rhs.y - y * rhs.x;
}
template<typename T>
bool vector2d<T>::operator==(const vector2d<T>& rhs) const{
    return x == rhs.x && y == rhs.y;
}
template<typename T>
bool vector2d<T>::operator!=(const vector2d<T>& rhs) const{
    return !(*this == rhs);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const vector2d<T>& vec){
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
}
#endif // VECTOR2D_H