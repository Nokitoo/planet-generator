cmake_minimum_required(VERSION 3.0)
set(EXECUTABLE_NAME "planet_generator")
project(${EXECUTABLE_NAME})

# include the config file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)


# List all source files
file(
  GLOB_RECURSE
  source_files
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*
  ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/*.cpp
)

# Find SDL2
# TODO: Use FindSDL2.cmake and support linux
if (OS_WINDOWS)
  set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/SDL2/include")
  set(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}/external/SDL2/lib/windows/${ARCH_DIR}/SDL2.lib")
  set(SDL2_DLL "${CMAKE_SOURCE_DIR}/external/SDL2/lib/windows/${ARCH_DIR}/SDL2.dll")
endif()

# Find GLEW
# TODO: Use FindGLEW.cmake and support linux
if (OS_WINDOWS)
  set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/glew/include")
  set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/external/glew/lib/windows/${ARCH_DIR}/glew32.lib")
  set(GLEW_DLL "${CMAKE_SOURCE_DIR}/external/glew/bin/windows/${ARCH_DIR}/glew32.dll")
endif()

# Find Opengl library
find_package(OpenGL REQUIRED)

# Add project includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add external includes
include_directories(
  ${SDL2_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/external/imgui/
  ${CMAKE_SOURCE_DIR}/external/glm/
  ${CMAKE_SOURCE_DIR}/external/stb_image/
  ${GLEW_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
)

# Create executable
add_executable(
  ${EXECUTABLE_NAME}
  ${source_files}
)

# Link libraries with executable
target_link_libraries(
  ${EXECUTABLE_NAME}
  ${SDL2_LIBRARY}
  ${GLEW_LIBRARY}
  ${OPENGL_gl_LIBRARY}
)

#Copy resources to build directory
file(
  COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/resources
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)

#Copy dlls to build directory
file(
  COPY
  ${SDL2_DLL}
  ${GLEW_DLL}
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)
