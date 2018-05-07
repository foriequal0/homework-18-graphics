//
// Created by foriequal0 on 18. 4. 1.
//

#ifndef SNU_GRAPHICS_DRAWABLES_H
#define SNU_GRAPHICS_DRAWABLES_H

#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/OpenGLSupport>

#include "Const.hpp"
#include "SweptSurfaceData.hpp"

class Drawable {
public:
  virtual void draw() const = 0;
};

class Cube final: public Drawable {
public:
  inline void draw() const override {
    glutSolidCube(1);
  }
};

class Sphere final: public Drawable {
public:
  inline void draw() const override {
    glutSolidSphere(1, 16, 16);
  }
};

class Cylinder final: public Drawable {
public:
  inline void draw() const override {
    const auto slices = 16;

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_POLYGON);
    glVertex3f(0, 0.5f, 0);
    for(auto i=0; i<=slices; i++) {
      auto rad = static_cast<float>(2*M_PI*i/slices);
      glVertex3f(sinf(rad)/2, 0.5f, cosf(rad)/2);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(auto i=0; i<=slices; i++) {
      auto rad = static_cast<float>(2*M_PI*i/slices);
      glVertex3f(sinf(-rad)/2, -0.5f, cosf(-rad)/2);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for(auto i=0; i<=slices; i++) {
      auto rad = static_cast<float>(2*M_PI*i/slices);
      glVertex3f(sinf(rad)/2, 0.5f, cosf(rad)/2);
      glVertex3f(sinf(rad)/2, -0.5f, cosf(rad)/2);
    }
    glEnd();
    glPopAttrib();
  }
};

class Cone final: public Drawable {
public:
  inline void draw() const override {
    glutSolidCone(1, 1, 16, 1);
  }
};

class Torus final: public Drawable {
  GLdouble innerRadius;
public:
  explicit Torus(GLdouble innerRadius) : innerRadius(innerRadius) {}

  inline void draw() const override {
    glutSolidTorus(innerRadius, 1, 8, 16);
  }
};

class Teapot final: public Drawable {
public:
  inline void draw() const override {
    glFrontFace(GL_CW);
    glutSolidTeapot(1);
    glFrontFace(GL_CCW);
  }
};

class Axis final: public Drawable {
  GLfloat size;
public:
  explicit Axis(GLfloat size = 1) : size(size) {}

  inline void draw() const override {
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

struct Vertex
{
  GLfloat position[3];
  GLfloat normal[3];

  Vertex(Eigen::Vector3f p, Eigen::Vector3f n) {
    position[0] = p[0];
    position[1] = p[1];
    position[2] = p[2];
    normal[0] = n[0];
    normal[1] = n[1];
    normal[2] = n[2];
  }
};

class SweptSurface final: public Drawable {
  std::vector<Vertex> vertices;
public:
  void draw() const override;

  static SweptSurface create(const SweptSurfaceData &data);
};

#endif //SNU_GRAPHICS_DRAWABLES_H
