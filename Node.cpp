//
// Created by foriequal0 on 18. 4. 1.
//

#include <algorithm>
#include "Node.hpp"

Node::Node(
        std::string name,
        Transform transform,
        std::shared_ptr<Drawable> drawable,
        std::vector<std::shared_ptr<Node>> childs)
        : name(std::move(name))
        , transform(std::move(transform))
        , drawable(std::move(drawable))
        , childs(std::move(childs))
{
    for(auto&& child: childs) {
        childs_by_name.emplace(child->name, child);
    }
}

void Node::insert(const std::shared_ptr<Node>& node) {
    assert(node->parent.expired());
    node->parent = this->shared_from_this();
    childs.push_back(node);
    childs_by_name.emplace(node->name, node);
}

void Node::insert(std::initializer_list<std::shared_ptr<Node>> nodes) {
    assert(std::all_of(std::begin(nodes), std::end(nodes), [](auto&& node) {
        return node->parent.expired();
    }));

    for(auto&& node: nodes){
        this->insert(node);
    }
}

void Node::draw() {
    auto guard = transform.guard();
    if (drawable != nullptr) {
        drawable->draw();
    }
    for (auto &&child: childs) {
        child->draw();
    }
}

auto Node::operator[](const std::string & name) -> std::shared_ptr<Node> {
    auto it = childs_by_name.find(name);
    if (it != childs_by_name.end()) {
        return (*it).second;
    }
    return nullptr;
}

auto Node::operator[](unsigned long index) -> std::shared_ptr<Node> {
    unsigned long i=0;
    for(auto it = std::begin(childs); i < index && it != std::end(childs); i++, it++) {
        auto&& child = *it;
        if (i == index) {
            return child;
        }
    }
    return nullptr;
}