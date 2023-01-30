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
PYBIND11_NUMPY_DTYPE(linalg3D::vector<double>, e1, e2, e3);
PYBIND11_NUMPY_DTYPE(linalg3D::dataPoint<double>, position, data);

auto build_grid_from_buffer(py::array_t<linalg3D::dataPoint<double>> data_)->grid<double>{
  linalg3D::dataPoint<double>* dataPoints=static_cast<linalg3D::dataPoint<double>*>(data_.ptr);
  return grid(dataPoints,data_.shape(0),data_.shape(1),data_.shape(3));
}

auto build_grid_from_separate_buffers(py::array_t<linalg3D::vector<double>> position_, py::array_t<double> data_)->grid<double>{
  double* data=static_cast<double*>(data_.ptr);
  linalg3D::vector<double>* position=static_cast<linalg3D::vector<double>*>(position_.ptr);
  return grid(position,data,data_.shape(0),data_.shape(1),data_.shape(3));
}

PYBIND11_MODULE(tesselate, m){
  py::class_<grid<double>>(m,"grid")
    .def(py::init<>())
    .def("as_grid",)
    .def("tesselate",&grid<double>::tesselate)
    .def_buffer([](grid<double>&g)->py::buffer_info{
      return py::buffer_info(
            g.dataPoints.data(),                                            // Pointer to buffer 
            sizeof(linalg3D::dataPoint<double>),                            // Size of one datapoint 
            py::format_descriptor<double>::format(),                        // Python struct-style format descriptor 
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
            py::format_descriptor<double>::format(),                     // Python struct-style format descriptor 
            1,{},{}              
        );
    }); 
    
}