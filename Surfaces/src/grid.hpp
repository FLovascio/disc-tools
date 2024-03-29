#pragma once

#include <vector>
#include <array>
#include <functional>
#include "cubes.hpp"
#include "linalg.hpp"
#include "triangles.hpp"
#include "marching_cubes.hpp"

// Will implement strips and sheets later for now here are placeholders
template <typename T>
struct strip{
  std::vector<linalg3D::dataPoint<T>> dataPoints;
  unsigned int nx;
};
template <typename T>
struct sheet{
  std::vector<linalg3D::dataPoint<T>> dataPoints;
  unsigned int nx;
  unsigned int ny;
};

template <typename T>
struct grid{
  std::vector<linalg3D::dataPoint<T>> dataPoints;
  unsigned int nx;
  unsigned int ny;
  unsigned int nz;
  grid(){
    dataPoints.reserve(32768);
    nx=32;
    ny=32;
    nz=32;
  }
  //c++ constructors
  grid(std::vector<linalg3D::dataPoint<T>> dataPoints_,unsigned int nx_, unsigned int ny_, unsigned int nz_):dataPoints(dataPoints_),nx(nx_),ny(ny_),nz(nz_){;}
  grid(std::vector<linalg3D::vector<T>> position_, std::vector<T> data_, unsigned int nx_, unsigned int ny_, unsigned int nz_):nx(nx_),ny(ny_),nz(nz_){
    dataPoints.reserve(data_.size());
    for(int i=0;i<data_.size();++i){
      dataPoints.push_back(linalg3D::dataPoint<T>{position_[i],data_[i]});
    }
  }
  //c constructors
  grid(linalg3D::dataPoint<T>* dataPoints_,unsigned int nx_, unsigned int ny_, unsigned int nz_):nx(nx_),ny(ny_),nz(nz_),dataPoints(dataPoints_,dataPoints_+(nx_*ny_*nz_)){;}
  grid(linalg3D::vector<T>* position_,T* data_,unsigned int nx_, unsigned int ny_, unsigned int nz_):nx(nx_),ny(ny_),nz(nz_){
    auto size=nx*ny*nz;
    dataPoints.reserve(size);
    for(int i=0;i<size;++i){
      dataPoints.push_back(linalg3D::dataPoint<T>{position_[i],data_[i]});
    }
  }
  grid(T* position_,T* data_,unsigned int nx_, unsigned int ny_, unsigned int nz_){
    auto size=nx*ny*nz;
    dataPoints.reserve(size);
    for(int i=0;i<size;++i){
      uint j=3*i;
      linalg3D::vector<T> position{position_[j],position_[j+1],position_[j+2]};
      dataPoints.push_back(linalg3D::dataPoint<T>{position,data_[i]});
    } 
  }
  static auto grid_from_ptr(linalg3D::dataPoint<T>* dataPoints_,unsigned int nx_, unsigned int ny_, unsigned int nz_)->grid<T>{
    return grid(dataPoints_,nx_,ny_,nz_);
  }
  static auto grid_from_position_value_ptr(linalg3D::vector<T>* position_,T* data_,unsigned int nx_, unsigned int ny_, unsigned int nz_)->grid<T>{
    return grid(position_,data_,nx_,ny_,nz_);
  }
  static auto grid_from_Tptr_Tptr(T* position_,T* data_,unsigned int nx_, unsigned int ny_, unsigned int nz_)->grid<T>{
    return grid(position_,data_,nx_,ny_,nz_);
  }
  inline auto size()->unsigned int{
    return dataPoints.size();
  }
  inline auto operator[](unsigned int i_)->linalg3D::dataPoint<T>&{
    return dataPoints[i_];
  }
  inline auto operator()(unsigned int i_,unsigned int j_,unsigned int k_)->linalg3D::dataPoint<T>&{
    return dataPoints[i_+(nx*j_)+(nx*ny*k_)];
  }
  inline auto item_size()->decltype(sizeof(linalg3D::dataPoint<T>)){
    return sizeof(linalg3D::dataPoint<T>);
  }
  inline auto h()->linalg3D::vector<T>{
    return linalg3D::minus(dataPoints[1+nx+nx*ny],dataPoints[0]);
  }
  inline auto cube(unsigned int i_,unsigned int j_,unsigned int k_)->dataCube<T>{
    return dataCube<T>{{{(*this)(i_,j_,k_),(*this)(i_+1,j_,k_),(*this)(i_,j_+1,k_),(*this)(i_+1,j_+1,k_),
                         (*this)(i_,j_,k_+1),(*this)(i_+1,j_,k_+1),(*this)(i_,j_+1,k_+1),(*this)(i_+1,j_+1,k_+1)}}};
  }
  inline auto cube(unsigned int i_)const->dataCube<T>{
    return dataCube<T>{{{(*this)[i_],(*this)[i_+1],(*this)[i_+nx],(*this)[i_+nx+1],
                         (*this)[i_+ny*nx],(*this)[i_+1+nx*ny],(*this)[i_+nx+nx*ny],(*this)[i_+1+nx+nx*ny]}}};
  }
  inline auto cube(linalg3D::vector<int> index_)->dataCube<T>{
    int i_=index_.e1;
    int j_=index_.e2;
    int k_=index_.e3;
    return dataCube<T>{{{(*this)(i_,j_,k_),(*this)(i_+1,j_,k_),(*this)(i_,j_+1,k_),(*this)(i_+1,j_+1,k_),
                         (*this)(i_,j_,k_+1),(*this)(i_+1,j_,k_+1),(*this)(i_,j_+1,k_+1),(*this)(i_+1,j_+1,k_+1)}}};
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
  auto interpolated_data_at_position(linalg3D::vector<T> position_)->T{
    auto index=linalg3D::elementwise_int_divide(position_, this->h());
    return this->cube(index).interpolate_data_in_cube(position_);
  }
};

template<typename T>
auto get_all_triangles(const grid<T>& data_,T isoLevel_,triangles<T> & outputTriangles_)->void{
  data_.tesselate(isoLevel_,outputTriangles_);
}

