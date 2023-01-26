#include <iostream>
#include <memory>
#include "linalg.hpp"
#include "triangles.hpp"
#include "utils.hpp"
#include "cubes.hpp"
#include "marching_cubes.hpp"
#include "grid.hpp"

auto make_test_volume(unsigned int resolution)->std::unique_ptr<grid<double>>{
  std::vector<linalg3D::dataPoint<double>> points;
  double h=2.0/((double)resolution);
  double px=-1.0;
  double py=-1.0;
  double pz=-1.0;
  for(uint k=0;k<resolution;++k){
    double py=-1.0;
    for(uint j=0;j<resolution;++j){
      double px=-1.0;
      for(uint i=0;i<resolution;++i){
        double data=px*px+py*py+pz*pz;
        points.push_back(linalg3D::dataPoint<double>{linalg3D::vector<double>{px,py,pz},data});
        px+=h;
      }
      py+=h;
    }
    pz+=h;
  }
  return std::make_unique<grid<double>>(grid<double>{points,resolution,resolution,resolution});
}

auto main()->int{
  std::cout<<"tests!\n";
  linalg3D::vector<double> vec1{1.0,0.0,0.0};
  linalg3D::vector<double> vec2{0.0,1.0,0.0}; 
  linalg3D::vector<double> vec3{0.0,2.0,0.0};
  vutils::print(linalg3D::cross(vec1,vec2));
  vutils::print(linalg3D::dot(vec1,vec2)); 
  vutils::print(linalg3D::direction(vec3));
  vutils::print(triangle<double>(triangleSides<double>{vec1,vec2}).normal());
  std::cout<<"testing utils::next_pow2()\npower of 2 after 13 is "<<utils::next_pow2(13)<<"\n";
  std::cout<<"building volume\n";
  unsigned int res=3;
  auto volume=make_test_volume(res);
  std::cout<<"done\n";
  for(uint i=0;i<res*res*res;++i){
    vutils::print((*volume)[i]);
  }
  std::cout<<"reserving triangles\n";
  triangles<double> tris(1000);
  std::cout<<"done\n";
  std::cout<<"tesselating volume\n";
  volume->tesselate(0.8,tris);
  std::cout<<"done\n";
  std::cout<<"found "<< tris.get_len()<<" triangles\n";

}