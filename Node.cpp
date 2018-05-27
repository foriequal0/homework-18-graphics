//
// Created by foriequal0 on 18. 4. 1.
//

#include <algorithm>
#include <iostream>
#include "Node.hpp"

namespace snu_graphics {
std::shared_ptr<Node> Node::create(
    std::string name,
    Transform transform,
    std::shared_ptr<Drawable> drawable,
    std::vector<std::shared_ptr<Node>> childs) {
  assert(std::all_of(std::begin(childs), std::end(childs), [](const std::shared_ptr<Node> &child) {
    return child->parent.expired();
  }));

  auto node = std::make_shared<Node>();
  node->name = std::move(name);
  node->transform = std::move(transform);
  node->drawable = std::move(drawable);

  for (auto &&child: childs) {
    child->parent = node;
    node->childs_by_name.emplace(child->name, child);
  }
  node->childs = std::move(childs);
  return node;
}

void Node::insert(const std::shared_ptr<Node> &node) {
  assert(node->parent.expired());
  node->parent = this->shared_from_this();
  childs.push_back(node);
  childs_by_name.emplace(node->name, node);
}

void Node::insert(std::initializer_list<std::shared_ptr<Node>> nodes) {
  assert(std::all_of(std::begin(nodes), std::end(nodes), [](const std::shared_ptr<Node> &node) {
    return node->parent.expired();
  }));

  for (auto &&node: nodes) {
    this->insert(node);
  }
}

bool showaxis = getenv("AXIS") != nullptr;

void Node::draw(Eigen::Vector3f origin) {
  auto guard = transform.guard();
  auto node_local_origin = transform.to_local(origin);

  if (drawable != nullptr) {
    drawable->draw(node_local_origin);
  } else {
    if (showaxis) {
      Axis().draw(node_local_origin);
    }
  }
  for (auto &&child: childs) {
    child->draw(node_local_origin);
  }
}

auto Node::get(const std::string &name) -> std::shared_ptr<Node> {
  auto it = childs_by_name.find(name);
  if (it != childs_by_name.end()) {
    return it->second;
  }
  assert(false);
  return nullptr;
}

auto Node::get(unsigned long index) -> std::shared_ptr<Node> {
  if (index < childs.size()) {
    return childs[index];
  }
  assert(false);
  return nullptr;
}
auto Node::operator[](const std::string &name) -> std::shared_ptr<Node> {
  return get(name);
}

auto Node::operator[](size_t index) -> std::shared_ptr<Node> {
  return get(index);
}
}