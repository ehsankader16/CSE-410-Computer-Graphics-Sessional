#ifndef UTILS_HPP
#define UTILS_HPP
#include "vector.hpp"

#define EPSILON 1e-5

struct Light {
  Vector position;
  Vector lookAt;
  double fallOff;
  double cutOffAngle;
};

struct Color {
  double r, g, b;

  Color operator+(const Color &c) const {
    double r = std::min(1.0, this->r + c.r);
    double g = std::min(1.0, this->g + c.g);
    double b = std::min(1.0, this->b + c.b);
    return {r, g, b};
  }

  Color operator*(const double& d) const {
    return {r * d, g * d, b * d};
  }
};

#endif