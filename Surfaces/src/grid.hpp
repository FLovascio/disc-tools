#pragma once

#include <vector>
#include <array>
#include <functional>
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
  grid(std::vector<linalg3D::dataPoint<T>> dataPoints_,unsigned int nx_, unsigned int ny_, unsigned int nz_):dataPoints(dataPoints_),nx(nx_),ny(ny_),nz(nz_){;}
  grid(std::vector<linalg3D::vector<T>> position_, std::vector<T> data_, unsigned int nx_, unsigned int ny_, unsigned int nz_):nx(nx_),ny(ny_),nz(nz_){
    dataPoints.reserve(data_.size());
    for(int i=0;i<data_.size();++i){
      dataPoints.push_back(linalg3D::dataPoint<T>{position_,data_});
    }
  }
  inline auto operator[](unsigned int i_)->linalg3D::dataPoint<T>&{
    return dataPoints[i_];
  }
  inline auto operator()(unsigned int i_,unsigned int j_,unsigned int k_)->linalg3D::dataPoint<T>&{
    return dataPoints[i_+(nx*j_)+(nx*ny*k_)];
  }
  inline auto cube(unsigned int i_,unsigned int j_,unsigned int k_)->dataCube<T>{
    return dataCube<T>{{{(*this)(i_,j_,k_),(*this)(i_+1,j_,k_),(*this)(i_,j_+1,k_),(*this)(i_+1,j_+1,k_),
                         (*this)(i_,j_,k_+1),(*this)(i_+1,j_,k_+1),(*this)(i_,j_+1,k_+1),(*this)(i_+1,j_+1,k_+1)}}};
  }
  inline auto cube(unsigned int i_)const->dataCube<T>{
    return dataCube<T>{{{(*this)[i_],(*this)[i_+1],(*this)[i_+nx],(*this)[i_+nx+1],
                         (*this)[i_+ny*nx],(*this)[i_+1+nx*ny],(*this)[i_+nx+nx*ny],(*this)[i_+1+nx+nx*ny]}}};
  }
  auto construct_marching_cube(unsigned int i_,unsigned int j_,unsigned int k_,T isoLevel_)->marchingCube<T>{
    return marchingCube<T>(cube(i_,j_,k_),isoLevel_);
  }
  auto construct_marching_cube(unsigned int i_,T isoLevel_)->marchingCube<T>{
    return marchingCube<T>(cube(i_),isoLevel_);
  }
  auto find_triangles(unsigned int i_,unsigned int j_,unsigned int k_,T isoLevel_,triangles<T> &triangStore_)->void{
    construct_marching_cube(i_,j_,k_,isoLevel_).build_triangles(triangStore_);
  }
  auto find_triangles(unsigned int i_,T isoLevel_,triangles<T> &triangStore_)->void{
    construct_marching_cube(i_,isoLevel_).build_triangles(triangStore_);
  }
  auto tesselate(T isoLevel_, triangles<T> &triangStore_)->void{
    for(unsigned int k=0;k<nz-1;++k){
      for(unsigned int j=0;j<ny-1;++j){
        for(unsigned int i=0;i<nx-1;++i){
          find_triangles(i,j,k,isoLevel_,triangStore_);
        };
      };
    };
  }
};

template<typename T>
auto get_all_triangles(const grid<T>& data_,T isoLevel_,triangles<T> & outputTriangles_)->void{
  data_.tesselate(isoLevel_,outputTriangles_);
}