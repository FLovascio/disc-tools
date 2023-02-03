#pragma once
#import <fstream>
#import <string>
#import <sstream>
#import "grid.hpp"
#import "type_utils.hpp"

template<template<class> class container, typename T>
auto read_from_bin_file(container<T> data, std::string filename)->bool{
  std::ifstream readBuffer(filename, std::ios::in | std::ios::binary);
  if(!readBuffer) {
    std::cerr<< "IO Error: cannot open file!\n";
    return false;
   }
  readBuffer.seekg (0, readBuffer.end);
  int length = readBuffer.tellg();
  readBuffer.seekg (0, readBuffer.beg);
  char * buffer = new char [length];
  readBuffer.read(buffer,length);
  delete[] buffer;
  readBuffer.close();
  if(!(readBuffer.good())){
    std::cerr<<"IO Error: encountered unknown error while writing\n";
    
    return false;
  }
  return true;
}