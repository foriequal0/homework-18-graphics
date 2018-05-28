//
// Created by foriequal0 on 18. 5. 5.
//

#include <algorithm>
#include <iostream>
#include <stack>
#include <Eigen/OpenGLSupport>
#include "Drawables.hpp"
#include "BSP.hpp"

namespace snu_graphics {
void SweptSurface::draw(Eigen::Vector3f origin) const {
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

void Transparent::draw(Eigen::Vector3f view_origin) const {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );

  glPushAttrib(GL_LIGHTING_BIT);
  glDepthMask(GL_FALSE);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  const auto draw = [](const MaterialedTriangle&mt) {
    const float alpha=0.8;
    const GLfloat ambiant[4] = {mt.material.ambient[0], mt.material.ambient[1], mt.material.ambient[2], alpha };
    const GLfloat diffuse[4] = {mt.material.diffuse[0], mt.material.diffuse[1], mt.material.diffuse[2], alpha };
    const GLfloat specular[4] = {mt.material.specular[0], mt.material.specular[1], mt.material.specular[2], alpha };

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambiant);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mt.material.shineness);

    glVertexPointer(3,
                    GL_FLOAT,
                    sizeof(Vertex),
                    reinterpret_cast<const unsigned char *>(mt.triangle.data()) + offsetof(Vertex, position));
    glNormalPointer(GL_FLOAT,
                    sizeof(Vertex),
                    reinterpret_cast<const unsigned char *>(mt.triangle.data()) + offsetof(Vertex, normal));
    glDrawArrays(GL_TRIANGLES, 0, mt.triangle.size());
  };

  const BSP *bsp = &this->bsp;
  while(bsp != nullptr) {
    auto root = bsp->root.triangle;
    auto normal = (root[1].get_position() - root[0].get_position()).cross(root[2].get_position() - root[0].get_position());
    auto origin = root[0].get_position();

    bool front = normal.dot(view_origin - origin) > 0;

    const std::vector<MaterialedTriangle> &drawfirst = (front) ? bsp->back_triangles : bsp->front_triangles;
    for(auto mt: drawfirst) {
      draw(mt);
    }
    draw(bsp->root);
    bsp = (front) ? bsp->front_tree.get() : bsp->back_tree.get();
  }
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glDepthMask(GL_TRUE);
  glPopAttrib();
}

Transparent Transparent::create(const std::vector<MaterialedTriangle> &data) {
  auto tp = Transparent();
  tp.bsp = BSP::from_triangles(data);
  return tp;
}

Transparent sample_transparent() {
  const Eigen::Vector3f v[] = {
      { 0, 1, 0 },
      { 1, 1, 0 },
      { 1, 1, 1 },
      { 0, 1, 1 },
      { 0, 0, 0 },
      { 1, 0, 0 },
      { 1, 0, 1 },
      { 0, 0, 1 },
  };
  const Eigen::Vector3f v2[] = {
      { -1, 1, -1 },
      { 0, 1, -1 },
      { 0, 1, 0 },
      { -1, 1, 0 },
      { -1, 0, -1 },
      { 0, 0, -1 },
      { 0, 0, 0 },
      { -1, 0, 0 },
  };

  std::vector<MaterialedTriangle> data = {
      MaterialedTriangle(Triangle{ Vertex(v[0], up), Vertex(v[2], up), Vertex(v[1], up)}, gold ),
      MaterialedTriangle(Triangle{ Vertex(v[0], up), Vertex(v[3], up), Vertex(v[2], up)}, gold),

      MaterialedTriangle(Triangle{ Vertex(v[0], back), Vertex(v[1], back), Vertex(v[4], back)}, silver),
      MaterialedTriangle(Triangle{ Vertex(v[1], back), Vertex(v[5], back), Vertex(v[4], back)}, silver),

      MaterialedTriangle(Triangle{ Vertex(v[1], right), Vertex(v[2], right), Vertex(v[5], right)}, chrome),
      MaterialedTriangle(Triangle{ Vertex(v[2], right), Vertex(v[6], right), Vertex(v[5], right)}, chrome),

      MaterialedTriangle(Triangle{ Vertex(v[2], front), Vertex(v[3], front), Vertex(v[6], front)}, red_plastic),
      MaterialedTriangle(Triangle{ Vertex(v[3], front), Vertex(v[7], front), Vertex(v[6], front)}, red_plastic),

      MaterialedTriangle(Triangle{ Vertex(v[3], left), Vertex(v[0], left), Vertex(v[7], left)}, cyan_rubber),
      MaterialedTriangle(Triangle{ Vertex(v[0], left), Vertex(v[4], left), Vertex(v[7], left)}, cyan_rubber),

      MaterialedTriangle(Triangle{ Vertex(v[4], down), Vertex(v[5], down), Vertex(v[6], down)}, pearl),
      MaterialedTriangle(Triangle{ Vertex(v[4], down), Vertex(v[6], down), Vertex(v[7], down)}, pearl),


      MaterialedTriangle(Triangle{ Vertex(v2[0], up), Vertex(v2[2], up), Vertex(v2[1], up)}, pearl),
      MaterialedTriangle(Triangle{ Vertex(v2[0], up), Vertex(v2[3], up), Vertex(v2[2], up)}, pearl),

      MaterialedTriangle(Triangle{ Vertex(v2[0], back), Vertex(v2[1], back), Vertex(v2[4], back)}, red_plastic),
      MaterialedTriangle(Triangle{ Vertex(v2[1], back), Vertex(v2[5], back), Vertex(v2[4], back)}, red_plastic),

      MaterialedTriangle(Triangle{ Vertex(v2[1], right), Vertex(v2[2], right), Vertex(v2[5], right)}, cyan_rubber),
      MaterialedTriangle(Triangle{ Vertex(v2[2], right), Vertex(v2[6], right), Vertex(v2[5], right)}, cyan_rubber),

      MaterialedTriangle(Triangle{ Vertex(v2[2], front), Vertex(v2[3], front), Vertex(v2[6], front)}, silver),
      MaterialedTriangle(Triangle{ Vertex(v2[3], front), Vertex(v2[7], front), Vertex(v2[6], front)}, silver),

      MaterialedTriangle(Triangle{ Vertex(v2[3], left), Vertex(v2[0], left), Vertex(v2[7], left)}, chrome),
      MaterialedTriangle(Triangle{ Vertex(v2[0], left), Vertex(v2[4], left), Vertex(v2[7], left)}, chrome),

      MaterialedTriangle(Triangle{ Vertex(v2[4], down), Vertex(v2[5], down), Vertex(v2[6], down)}, gold),
      MaterialedTriangle(Triangle{ Vertex(v2[4], down), Vertex(v2[6], down), Vertex(v2[7], down)}, gold),
  };

  std::vector<MaterialedTriangle> tesselated = std::move(data);
  std::vector<MaterialedTriangle> tmp;
  for(int i=0; i<2; i++) {
    for (auto mt : tesselated) {
      auto a = mt.triangle[0];
      auto b = mt.triangle[1];
      auto c = mt.triangle[2];

      auto ab = Vertex((a.get_position() + b.get_position()) / 2.0f, (a.get_normal() + b.get_normal()) / 2.0f);
      auto bc = Vertex((b.get_position() + c.get_position()) / 2.0f, (b.get_normal() + c.get_normal()) / 2.0f);
      auto ca = Vertex((c.get_position() + a.get_position()) / 2.0f, (c.get_normal() + a.get_normal()) / 2.0f);

      tmp.emplace_back(MaterialedTriangle(Triangle{a, ab, ca}, mt.material));
      tmp.emplace_back(MaterialedTriangle(Triangle{ab, b, bc}, mt.material));
      tmp.emplace_back(MaterialedTriangle(Triangle{ca, bc, c}, mt.material));
      tmp.emplace_back(MaterialedTriangle(Triangle{ab, bc, ca}, mt.material));
    }
    tesselated = std::move(tmp);
  }

  return Transparent::create(tesselated);
}
}