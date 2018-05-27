//
// Created by foriequal0 on 18. 5. 27.
//

#ifndef SNU_GRAPHICS_UNIT_HPP
#define SNU_GRAPHICS_UNIT_HPP

#include <array>
#include <GL/gl.h>

namespace snu_graphics {

struct Vertex {
  GLfloat position[3];
  GLfloat normal[3];

  Vertex(Eigen::Vector3f p, Eigen::Vector3f n) {
    position[0] = p[0];
    position[1] = p[1];
    position[2] = p[2];
    normal[0] = n[0];
    normal[1] = n[1];
    normal[2] = n[2];
  }
};

using Triangle = std::array<Vertex, 3>;
}

#endif //SNU_GRAPHICS_UNIT_HPP
