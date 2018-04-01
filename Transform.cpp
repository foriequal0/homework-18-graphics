//
// Created by foriequal0 on 18. 3. 31.
//

#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/OpenGLSupport>
#include "Transform.hpp"

Transform::Guard::Guard(bool entered) : entered(entered) {}

Transform::Guard::~Guard() {
  if (entered) {
    exit();
  }
}

void Transform::Guard::exit() {
  glPopMatrix();
  entered = false;
}

Transform::Guard Transform::guard() const {
  glPushMatrix();
  glRotate(orientation);
  glScale(scale);
  glTranslate(position);
  return Transform::Guard(true);
}
