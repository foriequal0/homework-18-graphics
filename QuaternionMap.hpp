//
// Created by foriequal0 on 18. 5. 7.
//

#ifndef SNU_GRAPHICS_QUATERNIONMAP_H
#define SNU_GRAPHICS_QUATERNIONMAP_H

inline Eigen::Quaternionf expq(Eigen::Vector4f q)
{
  auto a = q.tail(3).norm();
  auto exp_w = expf(q[0]);

  if (a == 0.0f)
  {
    return Eigen::Quaternionf(exp_w, 0.0f, 0.0f, 0.0f);
  }

  Eigen::Quaternionf res;
  res.w() = exp_w * cosf(a);
  res.vec() = exp_w * sinf(a) * q.tail(3);

  return res;
}

inline Eigen::Vector4f logq(Eigen::Quaternionf q)
{
  auto exp_w = q.norm();
  auto w = logf(exp_w);
  auto a = acosf(q.w() / exp_w);

  if (a == 0.0f)
  {
    return Eigen::Vector4f(w, 0.0f, 0.0f, 0.0f);
  }
  Eigen::Vector3f v = q.vec() / exp_w / (sin(a) / a);
  Eigen::Vector4f res;
  res << w, v.x(), v.y(), v.z();
  return res;
}

#endif //SNU_GRAPHICS_QUATERNIONMAP_H
