//
// Created by foriequal0 on 18. 5. 27.
//

#ifndef SNU_GRAPHICS_BSP_HPP
#define SNU_GRAPHICS_BSP_HPP

#include <vector>
#include <memory>
#include "Unit.hpp"

namespace snu_graphics {
class BSP {
public:
  MaterialedTriangle root;
  std::vector <MaterialedTriangle> front_triangles;
  std::vector <MaterialedTriangle> back_triangles;
  std::shared_ptr<BSP> front_tree;
  std::shared_ptr<BSP> back_tree;
  static BSP from_triangles(const std::vector <MaterialedTriangle> &triangles);
};
}

#endif //SNU_GRAPHICS_BSP_HPP
