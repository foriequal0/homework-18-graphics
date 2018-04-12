project(snu_graphics)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "\
-g \
-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization \
-Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs \
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

include(ExternalProject)
set(EXTERNAL_INSTALL_DIR ${CMAKE_BINARY_DIR}/external)
include_directories(${EXTERNAL_INSTALL_DIR}/include)
link_directories(${EXTERNAL_INSTALL_DIR}/lib)

# https://stackoverflow.com/questions/21223163/how-to-tell-cmake-to-download-some-necessary-header-files-more-precisely-glm-ma
ExternalProject_Add(eigen
        GIT_REPOSITORY "https://github.com/eigenteam/eigen-git-mirror"
        GIT_TAG "c3327783ac21591d0e87cca487c61a4cdea8dc4d"
        GIT_PROGRESS ON
        CMAKE_CACHE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:STRING=${EXTERNAL_INSTALL_DIR}
            -DBUILD_TESTING:BOOL=OFF
        )

include_directories(${EXTERNAL_INSTALL_DIR}/include/eigen3)
include_directories(${EXTERNAL_INSTALL_DIR}/include/eigen3/unsupported)

set(SOURCE_FILES main.cpp Const.cpp Const.hpp Transform.cpp Drawables.hpp Drawables.hpp Node.cpp Node.hpp)
add_executable(snu_graphics ${SOURCE_FILES})
add_dependencies(snu_graphics eigen)
add_definitions(-DGL_GLEXT_PROTOTYPES=1)
target_link_libraries(snu_graphics
        m
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        )