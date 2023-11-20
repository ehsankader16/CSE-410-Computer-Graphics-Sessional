#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "vector.hpp"
#include "utils.hpp"

class Object {
private:
  Color color;
  double ambient, diffuse, reflection, specular;
  double shininess;
public:
  Object() {
    color = {0, 0, 0};
    ambient = 0;
    diffuse = 0;
    reflection = 0;
    specular = 0;
    shininess = 0;
  }

  Object(Color color, double ambient, double diffuse, double reflection, double specular, double shininess) {
    this->color = color;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->reflection = reflection;
    this->specular = specular;
    this->shininess = shininess;
  }
  /**
   * @param p: origin of the ray
   * @param d: direction of the ray (normalized)
   * @return: the distance from p to the intersection point
  */
  virtual double intersect_t(const Vector p, const Vector d) = 0;
  virtual Vector normal(const Vector p) = 0;
  virtual void draw() = 0;

  virtual Color getColor() {
    return color;
  }

  virtual Color getColor(Vector p) {
    return color;
  }

  virtual double getAmbient() {
    return ambient;
  }

  virtual double getDiffuse() {
    return diffuse;
  }

  virtual double getReflection() {
    return reflection;
  }

  virtual double getSpecular() {
    return specular;
  }

  virtual double getShininess() {
    return shininess;
  }
};

#endif // OBJECT_HPP