cmake_minimum_required(VERSION 3.10)

project(snake3D)
set(CMAKE_BUILD_RPATH ./build)

add_executable(snake3D src/snake3D.cxx)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_dependencies(snake3D OpenGL::OpenGL)
include_directories(${GLUT_INCLUDE_DIRS} )

target_include_directories(snake3D PUBLIC ./include)
target_link_libraries(snake3D m GL GLU glut)

target_link_libraries(snake3D OpenGL::OpenGL ${GLUT_LIBRARY} )
