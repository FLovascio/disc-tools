#pragma once

#include "linalg.hpp"
#include "triangles.hpp"
#include <vector>
#include <array>

template <typename T>
class vectorCube{
  private:
    std::array<linalg3D::vector<T>,8> vertices;
  public:
  vectorCube(std::array<linalg3D::vector<T>,8> vertices_):vertices(vertices_){;}
  auto operator()(int i_, int j_, int k_)->linalg3D::vector<T>&{
    return vertices[i_+2*j_+2*k_];
  }
};

template <typename T>
class scalarCube{
  private:
    std::array<T,8> vertices;
  public:
  scalarCube(std::array<T,8> vertices_):vertices(vertices_){;}
  auto operator()(int i_, int j_, int k_)->linalg3D::vector<T>&{
    return vertices[i_+2*j_+2*k_];
  }
};

template<>
class scalarCube<bool>{
  private:
    unsigned char vertices;
  public:
  template<typename T>
  scalarCube(scalarCube<T>vertexData_, T isolevel){
    vertices=0;
    if(vertexData_[0]<isolevel){vertices|=1;}
    if(vertexData_[1]<isolevel){vertices|=2;}
    if(vertexData_[2]<isolevel){vertices|=4;}
    if(vertexData_[3]<isolevel){vertices|=8;}
    if(vertexData_[4]<isolevel){vertices|=16;}
    if(vertexData_[5]<isolevel){vertices|=32;}
    if(vertexData_[6]<isolevel){vertices|=64;}
    if(vertexData_[7]<isolevel){vertices|=128;}
  }
};

template<class T>
struct dataCube{
  std::array<T,8> vertexData;
};
template<>
struct dataCube<double>{
  scalarCube<double> vertexData;
};
template<>
struct dataCube<linalg3D::vector<double>>{
  vectorCube<double> vertexData;
};

