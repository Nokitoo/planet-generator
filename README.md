# planet-generator
## The project
This project is a generator of planets generated procedurally.
For now, it has the following features:
- OpenGL backend
- Dynamic LOD using quadtrees:
All the vertices are generated at runtime on the CPU, and only vertices that are displayed on the screen are stored, which reduce the amount of memory used and speed up the quad trees traversal
- Live editing using imgui library (https://github.com/ocornut/imgui)
- Normal map generation from heightmap using sobel filter
The normal map is generated with a fragment shader because it seems to be the fastest way to generate it (see my [stackoverflow question](https://stackoverflow.com/questions/44323900/opengl-compute-shader-normal-map-generation-poor-performance))
- Sphere generation from cube
It permits the use of a cubemap texture, which gives better results than mapping a normal texture on a sphere
- Normals debug and wireframe mode using geometry shader

Notes: The heightmap is not procedurally generated yet, so it could take some times to load it when running the program. Also, prefer running the Release build for better performances.

![Game](https://raw.githubusercontent.com/Nokitoo/planet-generator/dev/images/planet_generator.PNG)
Planet editor

## Building with CMake on Windows

All the libraries used are included in the repository. You just need to run cmake, it will automatically link the libraries and copy the dlls in the build directory.

## Building with CMake on Linux (upcoming)

It cannot be compiled yet on linux, some work has to be done to link the libraries with cmake using FindXXX.cmake modules.
