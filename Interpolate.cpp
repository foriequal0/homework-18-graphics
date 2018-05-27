//
// Created by foriequal0 on 18. 5. 6.
//

#include "Interpolate.hpp"

namespace snu_graphics {
template<>
Eigen::Vector3f Bezier::interpolate<Eigen::Vector3f>(float t,
                                                     Eigen::Vector3f b0,
                                                     Eigen::Vector3f b1,
                                                     Eigen::Vector3f b2,
                                                     Eigen::Vector3f b3) {
  const Eigen::RowVector4f T{powf(t, 3), powf(t, 2), t, 1};
  Eigen::Matrix4f M;
  M <<
    -1, 3, -3, 1,
      3, -6, 3, 0,
      -3, 3, 0, 0,
      1, 0, 0, 0;
  Eigen::Matrix<float, 4, 3> G;
  G.row(0) = b0.transpose();
  G.row(1) = b1.transpose();
  G.row(2) = b2.transpose();
  G.row(3) = b3.transpose();
  return T * M * G;
}

template<>
float Bezier::interpolate<float>(float t, float b0, float b1, float b2, float b3) {
  const Eigen::RowVector4f T{powf(t, 3), powf(t, 2), t, 1};
  Eigen::Matrix4f M;
  M <<
    -1, 3, -3, 1,
      3, -6, 3, 0,
      -3, 3, 0, 0,
      1, 0, 0, 0;
  Eigen::Matrix<float, 4, 1> G{b0, b1, b2, b3};
  return T * M * G;
}

template<>
Eigen::Quaternionf DeCasteljau::interpolate<Eigen::Quaternionf>(float t,
                                                                Eigen::Quaternionf b0,
                                                                Eigen::Quaternionf b1,
                                                                Eigen::Quaternionf b2,
                                                                Eigen::Quaternionf b3) {
  Eigen::Quaternionf a00 = b0.slerp(t, b1);
  Eigen::Quaternionf a01 = b1.slerp(t, b2);
  Eigen::Quaternionf a02 = b2.slerp(t, b3);

  Eigen::Quaternionf a10 = a00.slerp(t, a01);
  Eigen::Quaternionf a11 = a01.slerp(t, a02);

  Eigen::Quaternionf a20 = a10.slerp(t, a11);
  return a20;
}

template<>
Eigen::Vector3f BSpline::approx<Eigen::Vector3f>(float t,
                                                 Eigen::Vector3f b0,
                                                 Eigen::Vector3f b1,
                                                 Eigen::Vector3f b2,
                                                 Eigen::Vector3f b3) {
  const Eigen::RowVector4f T{powf(t, 3), powf(t, 2), t, 1};
  Eigen::Matrix4f M;
  M <<
    -1, 3, -3, 1,
      3, -6, 3, 0,
      -3, 0, 3, 0,
      1, 4, 1, 0;
  Eigen::Matrix<float, 4, 3> G;
  G.row(0) = b0.transpose();
  G.row(1) = b1.transpose();
  G.row(2) = b2.transpose();
  G.row(3) = b3.transpose();
  return T * (1 / 6.0f) * M * G;
}

template<>
float BSpline::approx<float>(float t, float b0, float b1, float b2, float b3) {
  const Eigen::RowVector4f T{powf(t, 3), powf(t, 2), t, 1};
  Eigen::Matrix4f M;
  M <<
    -1, 3, -3, 1,
      3, -6, 3, 0,
      -3, 0, 3, 0,
      1, 4, 1, 0;
  const Eigen::Matrix<float, 4, 1> G{b0, b1, b2, b3};
  return T * (1 / 6.0f) * M * G;
}


Eigen::Quaternionf quaternion_catmull_rom(const std::vector<Eigen::Quaternionf>& points, int index, float t, bool closed) {
  assert((closed && index < (int)points.size())
             || (!closed && index < (int)points.size()-1));


  const auto& b0 = safe_get(points, closed, index-1);
  const auto& b1 = safe_get(points, closed, index);
  const auto& b2 = safe_get(points, closed, index+1);
  const auto& b3 = safe_get(points, closed, index+2);

  const auto& p0 = b1;
  Eigen::Quaternionf p1 = b1 * expq(logq(b0.conjugate() * b1)/6.0f);
  Eigen::Quaternionf p2 = b2 * expq(-logq(b1.conjugate() * b3)/6.0f);
  const auto& p3 = b2;

  return DeCasteljau::template interpolate<Eigen::Quaternionf>(t, p0, p1, p2, p3);
}

}