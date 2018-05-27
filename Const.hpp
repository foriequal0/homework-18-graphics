//
// Created by foriequal0 on 18. 4. 1.
//

#ifndef SNU_GRAPHICS_CONST_H
#define SNU_GRAPHICS_CONST_H

#include <Eigen/Dense>
#include "Unit.hpp"

namespace snu_graphics {
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

static const Material emerald = Material({ 0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.07568, 0.633, 0.727811}, 0.633);
static const Material jade = Material({ 0.135, 0.2225, 0.1575}, {0.54, 0.89, 0.63}, {0.63, 0.316228, 0.316228}, 0.316228);
static const Material obsidian = Material({ 0.05375, 0.05, 0.06625}, {0.18275, 0.17, 0.22525}, {0.22525, 0.332741, 0.328634}, 0.346435);
static const Material pearl = Material({ 0.25, 0.20725, 0.20725}, {1, 0.829, 0.829}, {0.829, 0.296648, 0.296648}, 0.296648);
static const Material ruby = Material({ 0.1745, 0.01175, 0.01175}, {0.61424, 0.04136, 0.04136}, {0.04136, 0.727811, 0.626959}, 0.626959);
static const Material turquoise = Material({ 0.1, 0.18725, 0.1745}, {0.396, 0.74151, 0.69102}, {0.69102, 0.297254, 0.30829}, 0.306678);
static const Material brass = Material({ 0.329412, 0.223529, 0.027451}, {0.780392, 0.568627, 0.113725}, {0.113725, 0.992157, 0.941176}, 0.807843);
static const Material bronze = Material({ 0.2125, 0.1275, 0.054}, {0.714, 0.4284, 0.18144}, {0.18144, 0.393548, 0.271906}, 0.166721);
static const Material chrome = Material({ 0.25, 0.25, 0.25}, {0.4, 0.4, 0.4}, {0.4, 0.774597, 0.774597}, 0.774597);
static const Material copper = Material({ 0.19125, 0.0735, 0.0225}, {0.7038, 0.27048, 0.0828}, {0.0828, 0.256777, 0.137622}, 0.086014);
static const Material gold = Material({ 0.24725, 0.1995, 0.0745}, {0.75164, 0.60648, 0.22648}, {0.22648, 0.628281, 0.555802}, 0.366065);
static const Material silver = Material({0.19225, 0.19225, 0.19225}, {0.50754, 0.50754, 0.50754}, {0.50754, 0.508273, 0.508273}, 0.508273);
static const Material black_plastic = Material({0.0, 0.0, 0.0}, {0.01, 0.01, 0.01}, {0.01, 0.50, 0.50}, 0.50);
static const Material cyan_plastic = Material({0.0, 0.1, 0.06}, {0.0, 0.50980392, 0.50980392}, {0.50980392, 0.50196078, 0.50196078}, 0.50196078);
static const Material green_plastic = Material({0.0, 0.0, 0.0}, {0.1, 0.35, 0.1}, {0.1, 0.45, 0.55}, 0.45);
static const Material red_plastic = Material({0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.0, 0.7, 0.6}, 0.6);
static const Material white_plastic = Material({0.0, 0.0, 0.0}, {0.55, 0.55, 0.55}, {0.55, 0.70, 0.70}, 0.70);
static const Material yellow_plastic = Material({0.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {0.0, 0.60, 0.60}, 0.50);
static const Material black_rubber = Material({0.02, 0.02, 0.02}, {0.01, 0.01, 0.01}, {0.01, 0.4, 0.4}, 0.4);
static const Material cyan_rubber = Material({0.0, 0.05, 0.05}, {0.4, 0.5, 0.5}, {0.5, 0.04, 0.7}, 0.7);
static const Material green_rubber = Material({0.0, 0.05, 0.0}, {0.4, 0.5, 0.4}, {0.4, 0.04, 0.7}, 0.04);
static const Material red_rubber = Material({0.05, 0.0, 0.0}, {0.5, 0.4, 0.4}, {0.4, 0.7, 0.04}, 0.04);
static const Material white_rubber = Material({0.05, 0.05, 0.05}, {0.5, 0.5, 0.5}, {0.5, 0.7, 0.7}, 0.7);
static const Material yellow_rubber = Material({0.05, 0.05, 0.0}, {0.5, 0.5, 0.4}, {0.4, 0.7, 0.7}, 0.04);
}

#endif //SNU_GRAPHICS_CONST_H
