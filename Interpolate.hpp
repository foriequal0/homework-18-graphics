//
// Created by foriequal0 on 18. 5. 6.
//

#ifndef SNU_GRAPHICS_INTERPOLATE_H
#define SNU_GRAPHICS_INTERPOLATE_H

#include <cmath>
#include <vector>
#include <cassert>
#include <Eigen/Dense>
#include "QuaternionMap.hpp"

namespace snu_graphics {
struct Bezier {
  template<typename T>
  static T interpolate(float t, T b0, T b1, T b2, T b3) ;
};

template<>
Eigen::Vector3f Bezier::interpolate<Eigen::Vector3f>(
    float t, Eigen::Vector3f b0, Eigen::Vector3f b1, Eigen::Vector3f b2, Eigen::Vector3f b3
);

template<>
float Bezier::interpolate<float>(float t, float b0, float b1, float b2, float b3);

struct DeCasteljau {
  template<typename T>
  static T interpolate(float t, T b0, T b1, T b2, T b3);
};

template<>
Eigen::Quaternionf DeCasteljau::interpolate<Eigen::Quaternionf>(float t, Eigen::Quaternionf b0, Eigen::Quaternionf b1, Eigen::Quaternionf b2, Eigen::Quaternionf b3);

struct BSpline {
  template<typename T>
  static T approx(float t, T b0, T b1, T b2, T b3);
};

template<>
Eigen::Vector3f BSpline::approx<Eigen::Vector3f>(float t, Eigen::Vector3f b0, Eigen::Vector3f b1, Eigen::Vector3f b2, Eigen::Vector3f b3);

template<>
float BSpline::approx<float>(float t, float b0, float b1, float b2, float b3);

template<typename T>
const T& safe_get(const std::vector<T>& points, bool closed, int i) {
  const auto size = (int)points.size();
  if (closed) {
    if (i<0) { return points[size + i]; }
    else if (i >= size) { return points[i - size]; }
  }
  else {
    if (i < 0) { return points[0]; }
    else if (i >= size) { return points.back(); }
  }
  return points[i];
}

template<typename Method, typename T>
T catmull_rom(const std::vector<T>& points, int index, float t, bool closed=true) {
  const auto size = (int)points.size();
  assert((closed && index < (int)points.size())
             || (!closed && index < (int)points.size()-1));

  T a0 = (safe_get(points, closed, index+1) - safe_get(points, closed, index-1))/2;
  T a1 = (safe_get(points, closed, index+2) - safe_get(points, closed, index))/2;

  T p0 = safe_get(points, closed, index);
  T p3 = safe_get(points, closed, index+1);
  T p1 = p0 + a0/3;
  T p2 = p3 - a1/3;

  return Method::template interpolate<T>(t, p0, p1, p2, p3);
}

Eigen::Quaternionf quaternion_catmull_rom(const std::vector<Eigen::Quaternionf>& points, int index, float t, bool closed=true);

template<typename T>
T bspline(const std::vector<T>& points, int index, float t, bool closed=true) {
  assert((closed && index < (int)points.size())
             || (!closed && index < (int)points.size()-1));

  auto p0 = safe_get(points, closed, index-1);
  auto p1 = safe_get(points, closed, index);
  auto p2 = safe_get(points, closed, index+1);
  auto p3 = safe_get(points, closed, index+2);

  return BSpline::template approx<T>(t, p0, p1, p2, p3);
}
}
#endif //SNU_GRAPHICS_INTERPOLATE_H
