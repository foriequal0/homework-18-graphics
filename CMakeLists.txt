project(snu_graphics)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp)
add_executable(snu_graphics ${SOURCE_FILES})
target_link_libraries(snu_graphics ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )