#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include "linalg.hpp"
#include "utils.hpp"


template<typename T>
struct triangleVertices{
  linalg3D::vector<T> A;
  linalg3D::vector<T> B;
  linalg3D::vector<T> C;
};
template<typename T>
struct triangleSides{
  linalg3D::vector<T> a;
  linalg3D::vector<T> b; 
  //third side of a triangle is always a+b (because vectors),
  //so no need to store it.
};

template<typename T>
auto sides(triangleVertices<T> vertices_)->triangleSides<T>{
  return triangleSides<T>{linalg3D::minus(vertices_.A,vertices_.B),linalg3D::minus(vertices_.A,vertices_.C)};
}

template<typename T>
auto vertices(triangleSides<T> sides_, linalg3D::vector<T> origin_=linalg3D::vector<T>{(T)0.0,(T)0.0,(T)0.0})->triangleVertices<T>{
  return triangleVertices<T>{origin_,linalg3D::plus(sides_.a,origin_),linalg3D::plus(sides_.b,origin_)};
}

template<typename T>
auto normal(triangleSides<T> sides_)->linalg3D::vector<T>{
  return linalg3D::direction(linalg3D::cross(sides_.a,sides_.b));
}

template<typename T>
auto normal(triangleVertices<T> vertices_)->linalg3D::vector<T>{
  triangleSides<T> sidesTemp=sides(vertices_);
  return normal(sidesTemp);
}

template<typename T>
auto centre(triangleVertices<T>)->linalg3D::vector<T>{
  
}

template <typename T>
class triangle{
  private:
  triangleSides<T> tSides;
  triangleVertices<T> tVertices;
  public:
  triangle(triangleVertices<T> tVertices_):tVertices(tVertices_){
    tSides=sides(tVertices);
  }
  triangle(triangleSides<T> tSides_,linalg3D::vector<T> origin_=linalg3D::vector<T>{(T)0.0,(T)0.0,(T)0.0}):tSides(tSides_){
    tVertices=vertices(tSides,origin_);
  }
  auto normal()->linalg3D::vector<T>{
    return ::normal<T>(tSides);
  }
};


template <typename T>
struct triangles{
  std::vector<triangleVertices<T>> trianglesStored;
  triangles(unsigned int reserveLength_=16){
    trianglesStored.reserve(utils::next_pow2(reserveLength_));
  };
  inline auto size()->unsigned int{
    return trianglesStored.size();
  }
  auto get_len()->unsigned int{
    return trianglesStored.size();
  };
  auto push_back(triangleVertices<T> triangle_)->void{
    trianglesStored.push_back(triangle_);
  };
  auto normal()->linalg3D::vectors<T>{
    linalg3D::vectors<T> ret=linalg3D::vectors<T>(this->get_len());
    for(const auto &item:trianglesStored){
      ret.push_back(normal(item));
    }
    return ret;
  };
  inline auto operator[](unsigned int i_)->triangleVertices<T>&{
    return trianglesStored[i_];
  }
  inline auto item_size()->decltype(sizeof(triangleVertices<T>)){
    return sizeof(triangleVertices<T>);
  }
  inline auto begin()->decltype(std::begin(trianglesStored)){
    return std::begin(trianglesStored); 
  }
  inline auto end()->decltype(std::end(trianglesStored)){
    return std::end(trianglesStored); 
  }
};

template<typename T>
struct triangle_centres{
  std::vector<linalg3D::vector<T>> centres;
  triangle_centres(triangles<T> triangles_){
    centres.reserve(triangles_.size());
    for(auto &triangle:triangles_){
      centres.push_back(centre(triangle));
    }
  }
};