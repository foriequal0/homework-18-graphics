//
// Created by foriequal0 on 18. 5. 5.
//

#include <algorithm>
#include <iostream>
#include <Eigen/OpenGLSupport>
#include "Drawables.hpp"

namespace snu_graphics {
void SweptSurface::draw() const {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3,
                  GL_FLOAT,
                  sizeof(Vertex),
                  reinterpret_cast<const unsigned char *>(vertices.data()) + offsetof(Vertex, position));
  glNormalPointer(GL_FLOAT,
                  sizeof(Vertex),
                  reinterpret_cast<const unsigned char *>(vertices.data()) + offsetof(Vertex, normal));
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

SweptSurface SweptSurface::create(const SweptSurfaceData &data) {
  auto ss = SweptSurface();
  auto triangles = data.sweep_surface();
  for(auto &t: triangles) {
    ss.vertices.emplace_back(t[0]);
    ss.vertices.emplace_back(t[1]);
    ss.vertices.emplace_back(t[2]);
  }
  return ss;
}
}