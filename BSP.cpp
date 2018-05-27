//
// Created by foriequal0 on 18. 5. 27.
//

#include <Eigen/Dense>
#include "BSP.hpp"

namespace snu_graphics {
size_t get_pivot(const std::vector<MaterialedTriangle> &triangles) {
  size_t min_idx = 0;
  int min_diff = (int)triangles.size();

  for(size_t i = 0; i<triangles.size(); i++) {
    const Eigen::Vector3f origin = triangles[i].triangle[0].get_position();
    const Eigen::Vector3f normal =
        (triangles[i].triangle[1].get_position() - origin)
            .cross(triangles[i].triangle[2].get_position() - origin)
            .normalized();

    int front = 0;
    int back = 0;
    for(size_t j=0; j<triangles.size(); j++) {
      if (i==j) continue;
      const auto tri = triangles[j].triangle;

      const float distances[3] = {
          (tri[0].get_position() - origin).dot(normal),
          (tri[1].get_position() - origin).dot(normal),
          (tri[2].get_position() - origin).dot(normal),
      };

      auto f = std::count_if(std::begin(distances), std::end(distances), [](float x) { return x > 0; });
      auto b = std::count_if(std::begin(distances), std::end(distances), [](float x) { return x < 0; });

      if (f == 0) { back++; }
      else if (b == 0) { front ++; }
      else { front++; back++; }
    }

    if (min_diff > abs(front-back)) {
      min_diff = abs(front-back);
      min_idx = i;
    }
  }

  return min_idx;
}

BSP BSP::from_triangles(const std::vector<MaterialedTriangle> &triangles) {
  assert(triangles.size() > 0);
  if (triangles.size() == 1) {
    BSP tmp;
    tmp.root = triangles[0];
    return tmp;
  }

  auto pivot_index = get_pivot(triangles);
  MaterialedTriangle pivot = triangles[pivot_index];
  std::vector<MaterialedTriangle> fronts, backs;

  const Eigen::Vector3f origin = pivot.triangle[0].get_position();
  const Eigen::Vector3f normal =
      (pivot.triangle[1].get_position() - origin)
          .cross(pivot.triangle[2].get_position() - origin)
          .normalized();

  for(size_t i=0; i<triangles.size(); i++) {
    if (pivot_index==i) continue;
    const auto tri = triangles[i].triangle;
    const auto mat = triangles[i].material;

    const float distances[3] = {
        (tri[0].get_position() - origin).dot(normal),
        (tri[1].get_position() - origin).dot(normal),
        (tri[2].get_position() - origin).dot(normal),
    };

    auto fv = std::count_if(std::begin(distances), std::end(distances), [](float x) { return x > 0; });
    auto bv = std::count_if(std::begin(distances), std::end(distances), [](float x) { return x < 0; });
    if (fv == 0) { backs.emplace_back(tri, mat); continue; }
    else if (bv == 0) { fronts.emplace_back(tri, mat);  continue; }

    if (fv == 1 && bv == 1) {
      Vertex a, b, c;
      bool front = true;
      if (distances[0] == 0) { a = tri[1]; b = tri[2], c = tri[0]; front = distances[2] > 0; }
      else if (distances[1] == 0) { a = tri[2]; b = tri[0]; c = tri[1]; front = distances[0] > 0;}
      else if (distances[2] == 0) { a = tri[0]; b = tri[1]; c = tri[2]; front = distances[1] > 0;}

      auto t = normal.dot(origin - b.get_position()) / normal.dot(a.get_position()-b.get_position());
      Eigen::Vector3f d_pos = b.get_position() + t * (a.get_position()-b.get_position());
      Eigen::Vector3f d_normal = b.get_normal() + t * (a.get_normal()-b.get_normal());
      auto d = Vertex(d_pos, d_normal);
      if (front) {
        fronts.emplace_back(Triangle { d, a, c }, mat);
        backs.emplace_back(Triangle { d, c, b }, mat);
      } else {
        backs.emplace_back(Triangle { d, a, c }, mat);
        fronts.emplace_back(Triangle { d, c, b }, mat);
      }
      continue;
    }

    Vertex a, b, c;
    bool front = true;
    if (distances[0] * distances[2] > 0) { a = tri[0]; b = tri[1], c = tri[2]; front = distances[1] > 0; }
    else if (distances[1] * distances[0] > 0) { a = tri[1]; b = tri[2]; c = tri[0]; front = distances[2] > 0;}
    else if (distances[2] * distances[1] > 0) { a = tri[2]; b = tri[0]; c = tri[1]; front = distances[0] > 0;}

    auto t1 = normal.dot(origin - b.get_position()) / normal.dot(a.get_position()-b.get_position());
    auto d1_pos = b.get_position() + t1 * (a.get_position()-b.get_position());
    auto d1_normal = b.get_normal() + t1 * (a.get_normal()-b.get_normal());
    auto d1 = Vertex(d1_pos, d1_normal);

    auto t2 = normal.dot(origin - b.get_position()) / normal.dot(c.get_position()-b.get_position());
    auto d2_pos = b.get_position() + t1 * (c.get_position()-b.get_position());
    auto d2_normal = b.get_normal() + t1 * (c.get_normal()-b.get_normal());
    auto d2 = Vertex(d2_pos, d2_normal);
    if (front) {
      fronts.emplace_back(Triangle { d1, b, c }, mat);
      backs.emplace_back(Triangle { a, d1, d2 }, mat);
      backs.emplace_back(Triangle { a, d2, c }, mat);
    } else {
      backs.emplace_back(Triangle { d1, b, c }, mat);
      fronts.emplace_back(Triangle { a, d1, d2 }, mat);
      fronts.emplace_back(Triangle { a, d2, c }, mat);
    }
  }

  BSP tmp;
  tmp.root = pivot;
  tmp.front_triangles = std::move(fronts);
  tmp.back_triangles = std::move(backs);
  if (tmp.front_triangles.size() > 0) {
    tmp.front_tree = std::make_shared<BSP>(BSP::from_triangles(tmp.front_triangles));
  }
  if (tmp.back_triangles.size() > 0) {
    tmp.back_tree = std::make_shared<BSP>(BSP::from_triangles(tmp.back_triangles));
  }
  return tmp;
}
}