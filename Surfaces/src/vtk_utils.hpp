#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "grid.hpp"
#include "triangles.hpp"
#include "type_utils.hpp"
#include "linalg.hpp"
#include "polygons.hpp"

namespace vtk{
  namespace POLYDATA{
    struct points{};
    struct vertices{};
  };
  namespace RECTILINEAR_GRID{
    struct coordinates{};
  };
  namespace STRUCTURED_GRID{
    struct points{};
  }
  template<class contentType>
  struct vtkContent{};
  template<>
  struct vtkContent<POLYDATA::points>{
    inline static const std::string identifier="# vtk DataFile Version 3.0\n";
    inline static const std::string header="Polygon vertex coordinates";
    inline static const std::string dataType="DATASET POLYDATA\nPOINTS n float\n";
    std::vector<linalg3D::vector<float>> data;
  };
  template<>
  struct vtkContent<POLYDATA::vertices>{
    inline static const std::string identifier="# vtk DataFile Version 3.0\n";
    inline static const std::string header="Polygon vertex list";
    inline static const std::string dataType="DATASET POLYDATA\nVERTICES n float\n";
    std::vector<polygon> data;
  };
}


/*  template<template<class> class container,typename T>
  struct vtkWrite{
    inline static const std::string identifier="# vtk DataFile Version 3.0\n";
    std::string header;
    std::string dataType;
    std::string data;
    std::string attributes; 
  };
  template<typename T>
  struct vtkWrite<triangles,T>{

    std::vtkContent
    std::string attributes; 
  };
*/