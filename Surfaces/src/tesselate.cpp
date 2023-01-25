#include <vector>
#include <array>
#include <functional>
#include <memory>
#include <pybind11/pybind11.h>
#include "cubes.hpp"
#include "linalg.hpp"
#include "triangles.hpp"
#include "marching_cubes.hpp"
#include "grid.hpp"

namespace py = pybind11;

template<typename T>
auto grid_from_array(py::buffer)->std::unique_ptr<grid<T>>{
  
}

PYBIND11_MODULE(tesselate, m){
  py::class_<grid><double>(m,"grid")
    .def(py::init<>())
    .def("tesselate",&grid::tesselate);
    .def_buffer([]()->py::buffer_info{

    });
    
}