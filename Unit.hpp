//
// Created by foriequal0 on 18. 5. 27.
//

#ifndef SNU_GRAPHICS_UNIT_HPP
#define SNU_GRAPHICS_UNIT_HPP

#include <array>
#include <GL/gl.h>
#include <Eigen/Dense>

namespace snu_graphics {

struct Vertex {
  GLfloat position[3];
  GLfloat normal[3];
  Vertex() = default;
  Vertex(Eigen::Vector3f p, Eigen::Vector3f n) {
    position[0] = p[0];
    position[1] = p[1];
    position[2] = p[2];
    normal[0] = n[0];
    normal[1] = n[1];
    normal[2] = n[2];
  }

  inline Eigen::Vector3f get_position() const {
    return Eigen::Vector3f { position[0], position[1], position[2] };
  }
  inline Eigen::Vector3f get_normal() const {
    return Eigen::Vector3f { normal[0], normal[1], normal[2] };
  }
};

using Triangle = std::array<Vertex, 3>;

struct Material {
  GLfloat ambient[3];
  GLfloat diffuse[3];
  GLfloat specular[3];
  GLfloat shineness;
  Material() = default;
  Material(std::array<GLfloat, 3> ambient, std::array<GLfloat, 3> diffuse, std::array<GLfloat, 3> specular, GLfloat shineness) {
    std::copy(std::begin(ambient), std::end(ambient), std::begin(this->ambient));
    std::copy(std::begin(diffuse), std::end(diffuse), std::begin(this->diffuse));
    std::copy(std::begin(specular), std::end(specular), std::begin(this->specular));
    this->shineness = shineness;
  }
};

struct MaterialedTriangle {
  Triangle triangle;
  Material material;
  MaterialedTriangle() = default;
  MaterialedTriangle(Triangle triangle, Material material): triangle(triangle), material(material) {}
};
}

#endif //SNU_GRAPHICS_UNIT_HPP
