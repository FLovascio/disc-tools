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
    if(verticexData_[0]<isolevel){vertices|=1};
    if(verticexData_[1]<isolevel){vertices|=2};
    if(verticexData_[2]<isolevel){vertices|=4};
    if(verticexData_[3]<isolevel){vertices|=8};
    if(verticexData_[4]<isolevel){vertices|=16};
    if(verticexData_[5]<isolevel){vertices|=32};
    if(verticexData_[6]<isolevel){vertices|=64};
    if(verticexData_[7]<isolevel){vertices|=128};
  }
};

class cube{
  private:
    scalarCube<double> data;
    vectorCube<double> position;

};
