#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"

class Sphere : public Object {
private:
  Vector center;
  double radius;

public:
  Sphere(Vector center, double radius, Color color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->center = center;
    this->radius = radius;
  }

  double intersect_t(const Vector p, const Vector d) {
    // Geometric calculation
    Vector centerToP = p - center;

    double tp = -centerToP.dot(d);

    if (tp < EPSILON) {
      return -1;
    }

    double squaredPerpendicularDistanceToRay = centerToP.dot(centerToP) - tp * tp;

    if (squaredPerpendicularDistanceToRay > radius * radius) {
      return -1;
    }

    double t = sqrt(radius * radius - squaredPerpendicularDistanceToRay);

    if (tp - t > -EPSILON) {
      return tp - t;
    } else if (tp + t > -EPSILON) {
      return tp + t;
    } else {
      return -1;
    }
    
    // vector calculation
    // Vector distFromCenter = p - center;
    // double b = distFromCenter.dot(d);
    // double c = distFromCenter.dot(distFromCenter) - radius * radius;

    // double delta = b * b - c;

    // if (delta < 0) {
    //   return -1;
    // }

    // double t1 = -b - sqrt(delta);
    // double t2 = -b + sqrt(delta);

    // if (t1 < EPSILON && t2 < EPSILON) {
    //   return -1;
    // }

    // if (t1 < EPSILON) {
    //   return t2;
    // }

    // if (t2 < EPSILON) {
    //   return t1;
    // }

    // return t1 < t2 ? t1 : t2;
  }

  Vector normal(const Vector p) {
    return (p - center).normalize();
  }
  
  Color getColor() {
    return Object::getColor();
  }
  
  Color getColor(Vector p) {
    return getColor();
  }
  
  void draw() {
    glColor3f(getColor().r, getColor().g, getColor().b);
    glPushMatrix();
    glTranslatef(center.x, center.y, center.z);
    glutSolidSphere(radius, 100, 100);
    glPopMatrix();
  }
};

#endif