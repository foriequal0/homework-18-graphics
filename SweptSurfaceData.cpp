//
// Created by foriequal0 on 18. 5. 5.
//

#include <fstream>
#include <iostream>
#include "SweptSurfaceData.hpp"
#include "QuaternionMap.hpp"

namespace snu_graphics {
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
    auto front = &buf.front();
    setg(front, front + 1, front + 1);
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