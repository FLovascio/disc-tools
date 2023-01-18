add_executable(tests tests.cpp linalg.hpp triangles.hpp utils.hpp)
pybind11_add_module(surfaces MODULE tesselate.cpp linalg.hpp triangles.hpp marching_cubes.hpp cubes.hpp grid.hpp utils.hpp)