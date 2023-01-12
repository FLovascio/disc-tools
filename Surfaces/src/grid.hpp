#pragma once

#include <vector>
#include <array>
#include "cubes.hpp"
#include "linalg.hpp"
#include "triangles.hpp"

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
};
