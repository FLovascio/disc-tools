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
};
namespace vutils
{
template <typename T>
class vectorOutput{
  std::vector<linalg3D::vector<T>> vectors;
};
template <typename T>
auto print(linalg3D::vector<T>v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_.e1<<","<<v_.e2<<","<<v_.e3<<")\n";
}
auto print(double v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_<<")\n";
}
auto print(float v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_<<")\n";
}
auto print(int v_,std::ostream& out_=std::cout)->void{
  out_<<"("<<v_<<")\n";
}
}; // namespace vutils
