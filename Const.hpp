//
// Created by foriequal0 on 18. 4. 1.
//

#ifndef SNU_GRAPHICS_CONST_H
#define SNU_GRAPHICS_CONST_H

#include <Eigen/Dense>

static const Eigen::Vector3f zero = {0.0, 0.0, 0.0};
static const Eigen::Vector3f ones = {1.0, 1.0, 1.0};

static const Eigen::Vector3f black = {0.0, 0.0, 0.0};
static const Eigen::Vector3f white = {1.0, 1.0, 1.0};
static const Eigen::Vector3f red = {1.0, 0.0, 0.0};
static const Eigen::Vector3f yellow = {1.0, 1.0, 0.0};
static const Eigen::Vector3f green = {0.0, 1.0, 0.0};
static const Eigen::Vector3f cyan = {0.0, 1.0, 1.0};
static const Eigen::Vector3f blue = {0.0, 0.0, 1.0};
static const Eigen::Vector3f magenta = {1.0, 0.0, 1.0};

static const Eigen::Vector3f up = {0.0, 1.0, 0.0};
static const Eigen::Vector3f down = {0.0, -1.0, 0.0};
static const Eigen::Vector3f left = {-1.0, 0.0, 0.0};
static const Eigen::Vector3f right = {1.0, 0.0, 0.0};
static const Eigen::Vector3f back = {0.0, 0.0, -1.0};
static const Eigen::Vector3f front = {0.0, 0.0, 1.0};

static const Eigen::Quaternionf identity = {1.0f, 0.0f, 0.0f, 0.0f};

#endif //SNU_GRAPHICS_CONST_H
