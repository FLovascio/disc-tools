#include <vector>
#include <array>
#include <functional>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "cubes.hpp"
#include "linalg.hpp"
#include "triangles.hpp"
#include "marching_cubes.hpp"
#include "grid.hpp"

namespace py = pybind11;

auto build_grid_from_buffer(py::array_t<linalg3D::dataPoint<double>> data_)->grid<double>{
  linalg3D::dataPoint<double>* dataPoints=reinterpret_cast<linalg3D::dataPoint<double>*>(data_.ptr());
  return grid(dataPoints,data_.shape(0),data_.shape(1),data_.shape(3));
}

auto build_grid_from_separate_buffers(py::array_t<linalg3D::vector<double>> position_, py::array_t<double> data_)->grid<double>{
  double* data=reinterpret_cast<double*>(data_.ptr());
  linalg3D::vector<double>* position=reinterpret_cast<linalg3D::vector<double>*>(position_.ptr());
  return grid(position,data,data_.shape(0),data_.shape(1),data_.shape(3));
}

auto build_grid_from_array(py::array_t<double> position_, py::array_t<double> data_, uint nx, uint ny, uint nz)->grid<double>{
  double* data=reinterpret_cast<double*>(data_.ptr());
  double* position=reinterpret_cast<double*>(position_.ptr());
  return grid(position,data,nx,ny,nz);
}

PYBIND11_MODULE(tesselate, m){
  PYBIND11_NUMPY_DTYPE(linalg3D::vector<double>, e1, e2, e3);
  PYBIND11_NUMPY_DTYPE(linalg3D::dataPoint<double>, position, data);
  m.def("as_grid",&build_grid_from_array);
  py::class_<grid<double>>(m,"grid",py::buffer_protocol())
    .def(py::init(&build_grid_from_array))
    .def("tesselate",&grid<double>::tesselate)
    .def_buffer([](grid<double>&g)->py::buffer_info {
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
  py::class_<triangles<double>>(m,"triangles",py::buffer_protocol())
    .def(py::init<>())
    .def_buffer([](triangles<double>&t)->py::buffer_info {
      return py::buffer_info(
            t.trianglesStored.data(),                                    // Pointer to buffer 
            sizeof(triangleVertices<double>),                            // Size of one datapoint 
            py::format_descriptor<double>::format(),                     // Python struct-style format descriptor 
            1,{},{}              
        );
    }); 
  
}