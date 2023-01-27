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

PYBIND11_MODULE(tesselate, m){
  py::class_<grid<double>>(m,"grid")
    .def(py::init<>())
    .def("tesselate",&grid<double>::tesselate)
    .def_buffer([](grid<double>&g)->py::buffer_info{
      return py::buffer_info(
            g.dataPoints.data(),                                            // Pointer to buffer 
            sizeof(linalg3D::dataPoint<double>),                            // Size of one datapoint 
            py::format_descriptor<double>::format(),   // Python struct-style format descriptor 
            3,                                                              // Number of dimensions 
            { g.nx, g.ny, g.nz},                                            // Buffer dimensions 
            { sizeof(linalg3D::dataPoint<double>) * g.nx,                   // Strides (in bytes) for each index 
              sizeof(linalg3D::dataPoint<double>) * g.nx*g.ny}              // Pitch (in bytes) for each index
        );
    });
  py::class_<triangles<double>>(m,"triangles")
    .def(py::init<>())
    .def_buffer([](triangles<double>&t)->py::buffer_info{
      return py::buffer_info(
            t.trianglesStored.data(),                                    // Pointer to buffer 
            sizeof(triangleVertices<double>),                            // Size of one datapoint 
            py::format_descriptor<double>::format(),   // Python struct-style format descriptor 
            1,{},{}              
        );
    }); 
    
}