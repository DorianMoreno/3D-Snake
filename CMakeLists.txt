cmake_minimum_required(VERSION 3.10)

project(snake3D)
add_executable(snake3D snake3D.cxx)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_dependencies(snake3D OpenGL::OpenGL)
include_directories(${GLUT_INCLUDE_DIRS} )

target_link_libraries(snake3D m GL GLU glut)

target_link_libraries(snake3D OpenGL::OpenGL ${GLUT_LIBRARY} )
