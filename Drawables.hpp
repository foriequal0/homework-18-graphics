//
// Created by foriequal0 on 18. 4. 1.
//

#ifndef SNU_GRAPHICS_DRAWABLES_H
#define SNU_GRAPHICS_DRAWABLES_H

#include <GL/glut.h>

class Drawable {
public:
    virtual void draw() = 0;
};

class Cube: public Drawable {
public:
    inline void draw() override {
        glutSolidCube(1);
    }
};

class Sphere: public Drawable {
public:
    inline void draw() override {
        glutSolidSphere(1, 16, 16);
    }
};

class Cone: public Drawable {
public:
    inline void draw() override {
        glutSolidCone(1, 1, 16, 1);
    }
};

class Torus: public Drawable {
    GLdouble innerRadius;
public:
    explicit Torus(GLdouble innerRadius): innerRadius(innerRadius) {}
    inline void draw() override {
        glutSolidTorus(innerRadius, 1, 8, 16);
    }
};

class Teapot: public Drawable {
public:
    inline void draw() override {
        glutSolidTeapot(1);
    }
};

#endif //SNU_GRAPHICS_DRAWABLES_H
