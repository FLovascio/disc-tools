#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "grid.hpp"
#include "triangles.hpp"
#include "type_utils.hpp"

template<template<class> class container, typename T>
auto write_to_bin_file(container<T> data, std::string filename)->bool{
  if constexpr (has_size_method<container<T>>::value){
    std::cerr<< "Type Error: your container does not define a container<T>.size() method, how am I supposed to ,know how much data to output?\n";
    return false;
  }
  std::ofstream writeBuffer(filename, std::ios::out | std::ios::binary);
  if(!writeBuffer) {
    std::cerr<< "IO Error: cannot open file!\n";
    return false;
   }
  for(int i=0;i<data.size();++i){
    writeBuffer.write(static_cast<char *>(&data[i]), data.item_size());
  }
  writeBuffer.close();
  if(!(writeBuffer.good())){
    std::cerr<<"IO Error: encountered unknown error while writing\n";
    return false;
  }
  return true;
}


template<template<class> class container, typename T>
auto write_to_txt_file(container<T> data, std::string filename)->bool{
  if constexpr (has_size_method<container<T>>::value){
    std::cerr<< "Type Error: your container does not define a container<T>.size() method, how am I supposed to ,know how much data to output?\n";
    return false;
  }
  std::ofstream writeBuffer(filename, std::ios::out | std::ios::binary);
  if(!writeBuffer) {
    std::cerr<< "IO Error: cannot open file!\n";
    return false;
   }
  for(int i=0;i<data.size();++i){
    writeBuffer.write(static_cast<char *>(&data[i]), data.item_size());
  }
  writeBuffer.close();
  if(!(writeBuffer.good())){
    std::cerr<<"IO Error: encountered unknown error while writing\n";
    return false;
  }
  return true;
}