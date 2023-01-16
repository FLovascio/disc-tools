#pragma once

#include <vector>
#include <array>
#include "cubes.hpp"
#include "linalg.hpp"
#include "triangles.hpp"
#include "marching_cubes.hpp"

template <typename T>
struct grid{
  std::vector<linalg3D::dataPoint<T>> dataPoints;
  unsigned int nx;
  unsigned int ny;
  unsigned int nz;
  inline auto operator[](unsigned int i_)->linalg3D::dataPoint<T>&{
    return dataPoints[i_];
  }
  inline auto operator()(unsigned int i_,unsigned int j_,unsigned int k_)->linalg3D::dataPoint<T>&{
    return dataPoints[i_+(nx*j_)+(nx*ny*k_)];
  }
  inline auto cube(unsigned int i_,unsigned int j_,unsigned int k_)const->dataCube<T>{
    return dataCube<T>{{{this(i_,j_,k_),this(i_+1,j_,k_),this(i_,j_+1,k_),this(i_+1,j_+1,k_),
                         this(i_,j_,k_+1),this(i_+1,j_,k_+1),this(i_,j_+1,k_+1),this(i_+1,j_+1,k_+1)}}};
  }
  auto construct_marching_cube(unsigned int i_,unsigned int j_,unsigned int k_)const->marchingCube<T>{
    //return
  }
};
