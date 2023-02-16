#pragma once
#include <vector>
#include <initializer_list>

struct polygon{
  const int nVertices;
  std::vector<int> vertices;
  polygon(std::initializer_list<int> vertices_):vertices(vertices_),nVertices(vertices_.size()){;}
};