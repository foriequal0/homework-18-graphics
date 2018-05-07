//
// Created by foriequal0 on 18. 5. 5.
//

#include <algorithm>
#include <iostream>
#include <Eigen/OpenGLSupport>
#include "Drawables.hpp"
#include "Interpolate.hpp"

#define DRAW_POLY

void SweptSurface::draw() const {
#ifdef DRAW_POINT
  const GLfloat w[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glPushAttrib(GL_LIGHTING_BIT);
  glLineWidth(1);
  glPointSize(4);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, w);
  glColor3f(1, 1, 1);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices.data() + offsetof(Vertex, position));
  glDrawArrays(GL_POINTS, 0, vertices.size());
  glDisableClientState(GL_VERTEX_ARRAY);

  glPopAttrib();
#endif
#ifdef DRAW_LINE
  const GLfloat w[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glPushAttrib(GL_LIGHTING_BIT);
  glLineWidth(1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, w);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices.data() + offsetof(Vertex, position));
  glDrawArrays(GL_LINES, 0, vertices.size());
  glDisableClientState(GL_VERTEX_ARRAY);

  glPopAttrib();
#endif
#ifdef DRAW_POLY
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const unsigned char *>(vertices.data()) + offsetof(Vertex, position));
  glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<const unsigned char *>(vertices.data()) + offsetof(Vertex, normal));
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

#endif
}

template<typename F>
std::vector<CrossSection> interpolate_cross_sections(F method, const std::vector<CrossSection> &cross_sections) {
  std::vector<CrossSection> interpolated;
  for(auto& cs: cross_sections) {
    CrossSection tmp;
    tmp.position = cs.position;
    tmp.rotation = cs.rotation;
    tmp.scale = cs.scale;

    for (size_t i=0; i<cs.points.size(); i++) {
      const int subdiv = 10;
      for (int j = 0; j < subdiv; j += 1) {
        auto t = (float)j/subdiv;
        auto u = method(cs.points, (int)i, t, true);
        tmp.points.push_back(u);
      }
    }
    interpolated.push_back(tmp);
  }
  return interpolated;
}

SweptSurface SweptSurface::create(const SweptSurfaceData &data) {
  std::vector<CrossSection> cross_sections;
  switch(data.curve_type) {
  case CurveType::CATMULL_ROM:
    cross_sections = interpolate_cross_sections(catmull_rom<Bezier, Eigen::Vector3f>, data.cross_sections); break;
  case CurveType ::BSPLINE:
    cross_sections = interpolate_cross_sections(bspline<Eigen::Vector3f>, data.cross_sections); break;
  default: assert(false);
  }

#define FILTER(from, to, expr) \
  std::transform(std::begin(from), std::end(from), std::back_inserter(to), expr);

  std::vector<float> scales;
  std::vector<Eigen::Vector3f> positions;
  std::vector<Eigen::Quaternionf> rotations;
  FILTER(data.cross_sections, scales, [](const CrossSection &cs) { return cs.scale; });
  FILTER(data.cross_sections, positions, [](const CrossSection &cs) { return cs.position; });
  FILTER(data.cross_sections, rotations, [](const CrossSection &cs) { return cs.rotation; });
#undef FILTER
  std::vector<std::vector<Eigen::Vector3f>> sweeps;
  for(size_t j=0; j<cross_sections[0].points.size(); j++) {
    std::vector<Eigen::Vector3f> sweep;
    for (auto &cross_section : cross_sections) {
      sweep.push_back(cross_section.points[j]);
    }
    sweeps.emplace_back(sweep);
  }

  std::vector<std::vector<Eigen::Vector3f>> points;
  for(size_t i = 0; i < cross_sections.size()-1; i += 1) {
    const auto &cs = cross_sections[i];
    const int subdiv = 10;
    for (int j = 0; (i != cross_sections.size() - 2) ? j < subdiv: j < subdiv+1; j += 1) {
      std::vector<Eigen::Vector3f> transformed;
      auto t = (float)j/subdiv;
      auto scale = catmull_rom<Bezier>(scales, (int)i, t, false);
      auto pos = catmull_rom<Bezier>(positions, (int)i, t, false);
      auto rot = quaternion_catmull_rom(rotations, (int)i, t, false);

      for (size_t k=0; k<cs.points.size(); k++) {
        auto p = catmull_rom<Bezier>(sweeps[k], int(i), t, false);

        Eigen::Vector3f v = pos + rot.toRotationMatrix() * (scale * p);
        transformed.emplace_back(v);
      }
      points.emplace_back(transformed);
    }
  }

  std::vector<Vertex> vertices;
#ifdef DRAW_POINT
  for(size_t i=0; i<points.size(); i++) {
    auto sz = points[i].size();
    for(size_t j=0; j < sz; j++) {
      auto k = (j+1) % sz;
      Eigen::Vector3f norm = {0,0,1};
      vertices.emplace_back(Vertex(points[i][j], norm));
// vertices.emplace_back(Vertex(points[i][k], norm));
    }
  }
#endif
#ifdef DRAW_LINE
  for(size_t i=0; i<points.size(); i++) {
    auto sz = points[i].size();
    for(size_t j=0; j < sz; j++) {
      auto k = (j+1) % sz;
      Eigen::Vector3f norm = {0,0,1};
      vertices.emplace_back(Vertex(points[i][j], norm));
      vertices.emplace_back(Vertex(points[i][k], norm));
    }
  }
#endif
#ifdef DRAW_POLY
  auto f = [&](Eigen::Vector3f a, Eigen::Vector3f b, Eigen::Vector3f c) {
    Eigen::Vector3f normal = (b-a).cross(c-a).normalized();
    vertices.emplace_back(Vertex(a, normal));
    vertices.emplace_back(Vertex(b, normal));
    vertices.emplace_back(Vertex(c, normal));

    vertices.emplace_back(Vertex(a, -normal));
    vertices.emplace_back(Vertex(c, -normal));
    vertices.emplace_back(Vertex(b, -normal));
  };
//
//  {
//    const auto& front = points.front();
//    for (size_t j = 1; j < front.size(); j++) {
//      auto k = (j + 1) % front.size();
//      f(front[0], front[k], front[j]);
//    }
//  }

  for(size_t i=0; i<points.size()-1; i++) {
    assert(points[i].size() == points[i+1].size());
    auto sz = points[i].size();
    for(size_t j=0; j< sz; j++) {
      auto k = (j+1) % sz;
      f(points[i][j], points[i][k], points[i+1][j]);
      f(points[i+1][j], points[i][k], points[i+1][k]);
    }
  }

//  {
//    const auto& back = points.back();
//    for (size_t j = 1; j < back.size(); j++) {
//      auto k = (j + 1) % back.size();
//      f(back[0], back[k], back[j]);
//    }
//  }
#endif
  auto ss = SweptSurface();
  ss.vertices = vertices;
  return ss;
}
