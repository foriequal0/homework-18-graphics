//
// Created by foriequal0 on 18. 5. 5.
//

#include <fstream>
#include <iostream>
#include "SweptSurfaceData.hpp"
#include "QuaternionMap.hpp"
#include "Interpolate.hpp"

namespace snu_graphics {

const int CROSS_SECTION_SUBDIV = 4;
const int AXIAL_SUBDIV = 4;

template<typename F>
std::vector<CrossSection> interpolate_cross_sections(F method, const std::vector<CrossSection> &cross_sections) {
  std::vector<CrossSection> interpolated;
  for (auto &cs: cross_sections) {
    CrossSection tmp;
    tmp.position = cs.position;
    tmp.rotation = cs.rotation;
    tmp.scale = cs.scale;

    for (size_t i = 0; i < cs.points.size(); i++) {
      for (int j = 0; j < CROSS_SECTION_SUBDIV; j += 1) {
        auto t = (float) j / CROSS_SECTION_SUBDIV;
        auto u = method(cs.points, (int) i, t, true);
        tmp.points.push_back(u);
      }
    }
    interpolated.push_back(tmp);
  }
  return interpolated;
}

struct AccumulatedNormal {
  Eigen::Vector3f normal_sum = Eigen::Vector3f {0, 0, 0};
  float weight_sum = 0;

  void add(Eigen::Vector3f n, float w = 1.0f) {
    normal_sum += n * w;
    weight_sum += w;
  }

  Eigen::Vector3f get() const {
    return normal_sum / weight_sum;
  }
};

std::vector<Triangle> SweptSurfaceData::sweep_surface() const {
  std::vector<CrossSection> cross_sections;
  switch (curve_type) {
  case CurveType::CATMULL_ROM:
    cross_sections = interpolate_cross_sections(catmull_rom<Bezier, Eigen::Vector3f>, this->cross_sections);
    break;
  case CurveType::BSPLINE:cross_sections = interpolate_cross_sections(bspline<Eigen::Vector3f>, this->cross_sections);
    break;
  default: assert(false);
  }

#define FILTER(from, to, expr) \
  std::transform(std::begin(from), std::end(from), std::back_inserter(to), expr);

  std::vector<float> scales;
  std::vector<Eigen::Vector3f> positions;
  std::vector<Eigen::Quaternionf> rotations;
  FILTER(this->cross_sections, scales, [](const CrossSection &cs) { return cs.scale; });
  FILTER(this->cross_sections, positions, [](const CrossSection &cs) { return cs.position; });
  FILTER(this->cross_sections, rotations, [](const CrossSection &cs) { return cs.rotation; });
#undef FILTER
  std::vector<std::vector<Eigen::Vector3f>> sweeps;
  for (size_t j = 0; j < cross_sections[0].points.size(); j++) {
    std::vector<Eigen::Vector3f> sweep;
    for (auto &cross_section : cross_sections) {
      sweep.push_back(cross_section.points[j]);
    }
    sweeps.emplace_back(sweep);
  }

  std::vector<std::vector<Eigen::Vector3f>> points;
  for (size_t i = 0; i < cross_sections.size() - 1; i += 1) {
    const auto &cs = cross_sections[i];
    for (int j = 0; (i != cross_sections.size() - 2) ? j < AXIAL_SUBDIV : j < AXIAL_SUBDIV + 1; j += 1) {
      std::vector<Eigen::Vector3f> transformed;
      auto t = (float) j / AXIAL_SUBDIV;
      auto scale = catmull_rom<Bezier>(scales, (int) i, t, false);
      auto pos = catmull_rom<Bezier>(positions, (int) i, t, false);
      auto rot = quaternion_catmull_rom(rotations, (int) i, t, false);

      for (size_t k = 0; k < cs.points.size(); k++) {
        auto p = catmull_rom<Bezier>(sweeps[k], int(i), t, false);

        Eigen::Vector3f v = pos + rot.toRotationMatrix() * (scale * p);
        transformed.emplace_back(v);
      }
      points.emplace_back(transformed);
    }
  }

  std::vector<std::vector<AccumulatedNormal>> normals;
  normals.resize(points.size());
  for (auto &row: normals) {
    row.resize(points[0].size());
  }

  for (size_t i = 0; i < points.size() - 1; i++) {
    assert(points[i].size() == points[i + 1].size());
    auto sz = points[i].size();
    for (size_t j = 0; j < sz; j++) {
      auto k = (j + 1) % sz;

      auto a = points[i][j];
      auto b = points[i+1][j];
      auto c = points[i][k];
      auto normal = (b - a).cross(c - a).normalized();
      normals[i][j].add(normal);
      normals[i+1][j].add(normal);
      normals[i][k].add(normal);
    }
  }

  std::vector<Triangle> triangles;
  for (size_t i = 0; i < points.size() - 1; i++) {
    auto sz = points[i].size();
    for (size_t j = 0; j < sz; j++) {
      auto k = (j + 1) % sz;

      triangles.emplace_back(Triangle {
        Vertex(points[i][j], normals[i][j].get()),
        Vertex(points[i+1][j], normals[i+1][j].get()),
        Vertex(points[i][k], normals[i][k].get()),
      });
      triangles.emplace_back(Triangle {
        Vertex(points[i+1][j], normals[i+1][j].get()),
        Vertex(points[i+1][k], normals[i+1][k].get()),
        Vertex(points[i][k], normals[i][k].get())
      });

      // back face
      triangles.emplace_back(Triangle {
        Vertex(points[i][j], -normals[i][j].get()),
        Vertex(points[i][k], -normals[i][k].get()),
        Vertex(points[i+1][j], -normals[i+1][j].get()),
      });
      triangles.emplace_back(Triangle {
        Vertex(points[i+1][j], -normals[i+1][j].get()),
        Vertex(points[i][k], -normals[i][k].get()),
        Vertex(points[i+1][k], -normals[i+1][k].get()),
      });
    }
  }
  return triangles;
}

class comment_filter_streambuf
    : public std::streambuf {
  std::streambuf *src;
  bool in_comment = false;
  std::vector<char> buf;
protected:
  int underflow() override final {
    buf.reserve(BUFSIZ);
    buf.resize(0);
    int ch;
    while ((ch = src->sbumpc()) != traits_type::eof()) {
      if (in_comment) {
        if (ch == '\r') {
          continue;
        }
        if (ch == '\n') {
          in_comment = false;
          buf.emplace_back(ch);
        }
      } else {
        if (ch == '#') {
          in_comment = true;
          continue;
        } else if (ch == '\r') {
          continue;
        } else {
          buf.emplace_back(ch);
        }
      }

      if (buf.size() == buf.capacity()) {
        break;
      }
    }
    setg(&buf.front(), &buf.front(), &*buf.end());

    if (buf.empty()) {
      return traits_type::eof();
    } else {
      return buf[0];
    }
  }

public:
  explicit comment_filter_streambuf(std::streambuf *src)
      : src(src), in_comment(false) {
    setg(nullptr, nullptr, nullptr);
  }
};

class comment_filter_istream : public std::istream {
  comment_filter_streambuf sb;
public:
  explicit comment_filter_istream(const std::istream &is)
      : std::istream(&sb), sb(is.rdbuf()) {}
};

SweptSurfaceData SweptSurfaceData::load(const std::string &filename) {
  std::fstream fs{filename, std::fstream::in};
  assert(fs);
  return load(fs);
}

SweptSurfaceData SweptSurfaceData::load(std::istream &s) {
  comment_filter_istream is(s);
  SweptSurfaceData tmp;
  {
    std::string type;
    is >> type;
    if (type == "BSPLINE") {
      tmp.curve_type = CurveType::BSPLINE;
    } else if (type == "CATMULL_ROM") {
      tmp.curve_type = CurveType::CATMULL_ROM;
    } else {
      throw std::range_error("Invalid curve type: " + type);
    }
  }

  int num_cross_section;
  int num_control_points;
  is >> num_cross_section >> num_control_points;
  tmp.num_control_points = num_control_points;
  for (int i = 0; i < num_cross_section; i++) {
    CrossSection cs;
    for (int j = 0; j < num_control_points; j++) {
      Eigen::Vector3f point;
      point << 0, 0, 0;
      is >> point.x() >> point.z();
      cs.points.emplace_back(point);
    }
    is >> cs.scale;
    float w, x, y, z;
    is >> w >> x >> y >> z;
    Eigen::Vector3f v = sinf(w / 2) * Eigen::Vector3f{x, y, z}.normalized();
    cs.rotation = Eigen::Quaternionf(cosf(w / 2), v[0], v[1], v[2]).normalized();
    is >> cs.position.x() >> cs.position.y() >> cs.position.z();

    tmp.cross_sections.push_back(cs);
  }

  return tmp;
}

}