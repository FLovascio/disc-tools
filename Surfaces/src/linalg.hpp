#pragma once

#include <cmath>
#include <iostream>
#include <ostream>
#include <vector>

namespace linalg3D{
template <typename T>
struct vector{
  T e1;
  T e2;
  T e3;
};

template <typename T>
auto cross(vector<T> a,vector<T> b)->vector<T>{
  return vector<T>{a.e2*b.e3-a.e3*b.e2, a.e3*b.e1-b.e1*a.e3, a.e1*b.e2-a.e2*b.e1};
}
template <typename T>
auto dot(vector<T> a,vector<T> b)->T{
  return a.e1*b.e1+a.e2*b.e2+a.e3*b.e3;
}
template <typename T>
auto minus(vector<T> a,vector<T> b)->vector<T>{
  return vector<T>{a.e1-b.e1, a.e2-b.e2, a.e3-b.e3};
}
template <typename T>
auto plus(vector<T> a,vector<T> b)->vector<T>{
  return vector<T>{a.e1+b.e1, a.e2+b.e2, a.e3+b.e3};
}
template <typename T>
auto norm(vector<T> a)->T{
  return sqrt(a.e1*a.e1+ a.e2*a.e2+ a.e3*a.e3);
}
template <typename T>
auto direction(vector<T> a)->vector<T>{
  T vNorm=norm(a);
  return vector<T>{a.e1/vNorm,a.e2/vNorm,a.e3/vNorm};
}
template <typename T>
auto elementwise_divide(vector<T> a,vector<T> b)->vector<T>{
  return vector<T>{a.e1/b.e1, a.e2/b.e2, a.e3/b.e3}; 
}
template <typename T>
auto elementwise_int_divide(vector<T> a,vector<T> b)->vector<int>{
  return vector<int>{static_cast<int>(a.e1/b.e1), static_cast<int>(a.e2/b.e2), static_cast<int>(a.e3/b.e3)}; 
}

template<typename T>
struct dataPoint{
  vector<T> position;
  T data;
};

template <typename T>
struct vectors{
  std::vector<vector<T>> vectorsStored;
  vectors(unsigned int reserveLength_=0){
    vectorsStored.reserve(reserveLength_);
  };  
  auto push_back(vector<T> vec_)->void{
    vectorsStored.push_back(vec_);
  };
};

};
namespace vutils
{
template <typename T>
class vectorOutput{
  std::vector<linalg3D::vector<T>> vectors;
};
template <typename T>
inline auto print(linalg3D::dataPoint<T>v_,std::ostream& out_=std::cout)->void{
  out_<<v_.data<<" ("<<v_.position.e1<<","<<v_.position.e2<<","<<v_.position.e3<<")\n";
}
template <typename T>
inline auto print(linalg3D::vector<T>v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_.e1<<","<<v_.e2<<","<<v_.e3<<")\n";
}
inline auto print(double v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_<<")\n";
}
inline auto print(float v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_<<")\n";
}
inline auto print(int v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_<<")\n";
}
}; // namespace vutils
