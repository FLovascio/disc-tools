#pragma once
#include <vector>
#include <initializer_list>
#include <string>
#include <sstream>

struct polygon{
  const int nVertices;
  std::vector<int> vertices;
  polygon(std::initializer_list<int> vertices_):vertices(vertices_),nVertices(vertices_.size()){;}
  auto to_string()->std::string{
    std::stringstream retString;
    retString<< nVertices;
    for(const int& vertex : vertices){
        retString <<","<< vertex;
    }
    return retString.str();
  }
};