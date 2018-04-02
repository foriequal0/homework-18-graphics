//
// Created by foriequal0 on 18. 4. 1.
//

#ifndef SNU_GRAPHICS_NODE_H
#define SNU_GRAPHICS_NODE_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <Eigen/Dense>

#include "Transform.hpp"
#include "Drawables.hpp"

class Node : public std::enable_shared_from_this<Node> {
public:
  std::string name;
  Transform transform;
  std::shared_ptr<Drawable> drawable;
  std::weak_ptr<Node> parent;
  std::vector<std::shared_ptr<Node>> childs;
  std::unordered_multimap<std::string, std::shared_ptr<Node>> childs_by_name;

public:
  Node() = default;
  Node(const Node &) = default;
  Node(Node &&) = default;
  Node &operator=(const Node &) = default;
  Node &operator=(Node &&) = default;

  static std::shared_ptr<Node> create(
      std::string name,
      Transform transform = Transform(),
      std::shared_ptr<Drawable> drawable = nullptr,
      std::vector<std::shared_ptr<Node>> childs = std::vector<std::shared_ptr<Node>>{}
  );

  void insert(const std::shared_ptr<Node> &node);
  void insert(std::initializer_list<std::shared_ptr<Node>> node);
  void draw();
  auto get(const std::string &) -> std::shared_ptr<Node>;
  auto get(size_t) -> std::shared_ptr<Node>;
  auto operator[](const std::string &) -> std::shared_ptr<Node>;
  auto operator[](size_t) -> std::shared_ptr<Node>;
};

#endif //SNU_GRAPHICS_NODE_H
