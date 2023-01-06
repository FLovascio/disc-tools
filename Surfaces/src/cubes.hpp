#pragma once

#include "linalg.hpp"
#include "triangles.hpp"
#include <vector>
#include <array>

template <typename T>
class neighbourCube{
  private:
    std::array<linalg3D::vector<T>,27> nodes;
  public:
  neighbourCube(std::array<linalg3D::vector<T>,27> nodes_):nodes(nodes_){;}
  auto operator()(int i_, int j_, int k_)->linalg3D::vector<T>&{
    return nodes[(i_+1)+3*(j_+1)+9*(k_+1)];
  }
};

template <typename T>
class vertexList{
  private:
    std::array<triangleVertices,28> triangles;
  public:
    
};