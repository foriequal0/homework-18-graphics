//
// Created by foriequal0 on 18. 4. 1.
//

#ifndef SNU_GRAPHICS_DRAWABLES_H
#define SNU_GRAPHICS_DRAWABLES_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/OpenGLSupport>

#include "Const.hpp"

class Drawable {
public:
  virtual void draw() = 0;
};

class Cube : public Drawable {
public:
  inline void draw() override {
    glutSolidCube(1);
  }
};

class Sphere : public Drawable {
public:
  inline void draw() override {
    glutSolidSphere(1, 16, 16);
  }
};

class Cone : public Drawable {
public:
  inline void draw() override {
    glutSolidCone(1, 1, 16, 1);
  }
};

class Torus : public Drawable {
  GLdouble innerRadius;
public:
  explicit Torus(GLdouble innerRadius) : innerRadius(innerRadius) {}

  inline void draw() override {
    glutSolidTorus(innerRadius, 1, 8, 16);
  }
};

class Teapot : public Drawable {
public:
  inline void draw() override {
    glFrontFace(GL_CW);
    glutSolidTeapot(1);
    glFrontFace(GL_CCW);
  }
};

class Axis : public Drawable {
  GLfloat size;
public:
  explicit Axis(GLfloat size=1): size(size) {}

  inline void draw() override {
    const GLfloat r[] = {1.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat g[] = {0.0f, 1.0f, 0.0f, 1.0f};
    const GLfloat b[] = {0.0f, 0.0f, 1.0f, 1.0f};
    glPushAttrib(GL_LIGHTING_BIT);
    glLineWidth(1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, r);
    glBegin(GL_LINES);
    glVertex(zero);
    glVertex(up);
    glEnd();

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, g);
    glBegin(GL_LINES);
    glVertex(zero);
    glVertex(right);
    glEnd();

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, b);
    glBegin(GL_LINES);
    glVertex(zero);
    glVertex(front);
    glEnd();
    glPopAttrib();
  }
};

#endif //SNU_GRAPHICS_DRAWABLES_H
