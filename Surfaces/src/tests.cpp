#include <iostream>
#include "linalg.hpp"
#include "triangles.hpp"

auto main()->int{
  std::cout<<"tests!\n";
  linalg3D::vector<double> vec1{1.0,0.0,0.0};
  linalg3D::vector<double> vec2{0.0,1.0,0.0}; 
  linalg3D::vector<double> vec3{0.0,2.0,0.0};
  vutils::print(linalg3D::cross(vec1,vec2));
  vutils::print(linalg3D::dot(vec1,vec2)); 
  vutils::print(linalg3D::direction(vec3));
  vutils::print(triangle<double>(triangleSides<double>{vec1,vec2}).normal());
}