project(snu_graphics)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "\
-g -Og \
-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization \
-Wformat=2 -Winit-self -Wmissing-include-dirs \
-Woverloaded-virtual -Wredundant-decls -Wsign-promo \
-Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused \
-fsanitize=address -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-op -Wnoexcept -Wstrict-null-sentinel")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
include_directories(eigen3 eigen3/unsupported)

set(SOURCE_FILES
        main.cpp
        Transform.cpp
        Drawables.cpp
        Node.cpp
        SweptSurfaceData.cpp
        Interpolate.cpp)
add_executable(snu_graphics ${SOURCE_FILES})
add_definitions(-DGL_GLEXT_PROTOTYPES=1)
target_link_libraries(snu_graphics
        m
        pthread
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        )