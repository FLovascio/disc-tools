#pragma once
#include "linalg.hpp"
#include "triangles.hpp"
#include <sstream>
#include <string>

template <typename T,typename numericType>
auto to_string(linalg3D::vector<T> vector_, std::string separator_ = "")
    -> std::string {
  std::stringstream rss;
  rss << static_cast<numericType>(vector_.e1) << separator_ << static_cast<numericType>(vector_.e2) << separator_ << static_cast<numericType>(vector_.e3);
  return rss.str();
}

template <typename T,typename numericType>
auto to_string(triangleVertices<T> triangle_, std::string vectorSeparator_ = "",
               std::string pointSeparator_ = "\n") -> std::string {
  std::stringstream rss;
  rss << to_string<numericType>(triangle_.A,vectorSeparator_) << pointSeparator_ << to_string<numericType>(triangle_.B,vectorSeparator_)
      << pointSeparator_ << to_string<numericType>(triangle_.C,vectorSeparator_);
  return rss.str();
}

template <typename T,typename numericType>
auto to_string(triangles<T> triangles_, std::string vectorSeparator_ = "",
               std::string pointSeparator_ = "\n",
               std::string triangleSeparator_ = "\n") -> std::string {
  std::stringstream rss;
  for(const triangleVertices<T>& triangle : triangles_){
    rss << to_string<numericType>(triangle,vectorSeparator_,pointSeparator_) << triangleSeparator_;
  }
  return rss.str();            
}