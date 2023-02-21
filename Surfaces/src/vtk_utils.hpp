#pragma once

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "grid.hpp"
#include "triangles.hpp"
#include "type_utils.hpp"
#include "linalg.hpp"
#include "polygons.hpp"
#include "io_helpers.hpp"

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
  struct vtkContent{
    inline static const std::string identifier="# vtk DataFile Version 3.0\n";
    std::string header="\n";
    std::string dataType="\n";
    std::string data="\n";
  };
  template<>
  struct vtkContent<POLYDATA::points>{
    inline static const std::string identifier="# vtk DataFile Version 3.0\n";
    inline static const std::string header="Polygon vertex coordinates\n";
    std::string dataType;
    std::string data;
    vtkContent(triangles<double> triangles_){
      data=to_string<double,float>(triangles_);
      std::stringstream dataTypeStream;
      dataTypeStream <<"DATASET POLYDATA\nPOINTS "<<3*triangles_.size()<<" float\n"; 
      dataType=dataTypeStream.str();
    } 
  };
  template<>
  struct vtkContent<POLYDATA::vertices>{
    // this struct works by sequentially adding polygons, which guarantees 
    // double counting of vertices
    inline static const std::string identifier="# vtk DataFile Version 3.0\n";
    inline static const std::string header="Polygon vertex list\n";
    std::string dataType;
    std::string data;
    vtkContent(uint nShapes_, uint vertices_=3, bool __noAlias__=false){
      if(__noAlias__){
        std::cerr<<"VTK Error: noalias mode for vtk POLYDATA is not yet supported\n";
        return;
      }
      std::stringstream dataTypeStream;
      dataTypeStream <<"DATASET POLYDATA\nVERTICES "<<nShapes_<<" float\n"; 
      dataType=dataTypeStream.str();
      std::stringstream dataStream;
      int side=0;
      for(int i=0;i<nShapes_;++i){
        dataStream << vertices_;
        for(int j=0;j<vertices_;++j){
          dataStream <<","<<side;
          side++;
        }
        dataStream << "\n";
      }
      data=dataStream.str();
    }
  };
  template<class contentType>
  auto to_file(vtkContent<contentType> content, std::string filename)->bool{
    std::ofstream outFile(filename);
    outFile<<content.identifier;
    outFile<<content.header;
    outFile<<content.dataType;
    outFile<<content.data;
    return true;
  }
  template<typename T>
  bool writeVTK(triangles<T> triangles_,std::string name){
    {
      vtkContent<POLYDATA::points> pointData(triangles_);
      to_file(pointData, name+"_points.vtk");
    }
    {
      vtkContent<POLYDATA::vertices> vertexData(triangles_.size()); 
      to_file(vertexData, name+"_vertices.vtk");
    }
    return true;
  }
}
