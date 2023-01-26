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
  auto operator[](unsigned int i_)->const linalg3D::vector<T>{
    return vertices[i_];
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
  auto operator[](unsigned int i_)->const T{
    return vertices[i_];
  }
};

template<>
class scalarCube<bool>{
  private:
    int vertices;
  public:
  scalarCube(){
    vertices=0;
  }
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
  auto get_value()->int{
    return this->vertices;
  }
};

template<typename T>
struct dataCube{
  std::array<linalg3D::dataPoint<T>,8> vertexData;
  inline auto data()const->scalarCube<T>{
    return scalarCube<T>{std::array<T,8>{vertexData[0].data,
                                 vertexData[1].data,
                                 vertexData[2].data,
                                 vertexData[3].data,
                                 vertexData[4].data,
                                 vertexData[5].data,
                                 vertexData[6].data,
                                 vertexData[7].data}};
  };
  inline auto position()const->vectorCube<T>{
    return vectorCube<T>{std::array<linalg3D::vector<T>,8>{vertexData[0].position,
                                 vertexData[1].position,
                                 vertexData[2].position,
                                 vertexData[3].position,
                                 vertexData[4].position,
                                 vertexData[5].position,
                                 vertexData[6].position,
                                 vertexData[7].position}};
  };
};