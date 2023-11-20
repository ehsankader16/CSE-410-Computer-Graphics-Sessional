#ifndef CUBE_HPP
#define CUBE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "square.hpp"

class Cube : public Object {
private:
  Vector bottomLowerLeft;
  double side;  
  Square faces[6];
public:
  Cube(Vector bottomLowerLeft, double side, Color color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->bottomLowerLeft = bottomLowerLeft;
    this->side = side;

    Vector topLowerRight(bottomLowerLeft.x + side, bottomLowerLeft.y, bottomLowerLeft.z + side);
    Vector topUpperRight(bottomLowerLeft.x + side, bottomLowerLeft.y + side, bottomLowerLeft.z + side);
    Vector topUpperLeft(bottomLowerLeft.x, bottomLowerLeft.y + side, bottomLowerLeft.z + side);
    Vector bottomUpperRight(bottomLowerLeft.x + side, bottomLowerLeft.y + side, bottomLowerLeft.z);

    faces[0] = Square(topUpperRight, Vector(1, 0, 0), side, color, ambient, diffuse, reflection, specular, shininess);
    faces[1] = Square(topLowerRight, Vector(0, -1, 0), side, color, ambient, diffuse, reflection, specular, shininess);
    faces[2] = Square(topUpperRight, Vector(0, 0, 1), side, color, ambient, diffuse, reflection, specular, shininess);
    faces[3] = Square(topUpperRight, Vector(0, 1, 0), side, color, ambient, diffuse, reflection, specular, shininess);
    faces[4] = Square(topUpperLeft, Vector(-1, 0, 0), side, color, ambient, diffuse, reflection, specular, shininess);
    faces[5] = Square(bottomUpperRight, Vector(0, 0, -1), side, color, ambient, diffuse, reflection, specular, shininess);
  }

  double intersect_t(Vector p, Vector d) {
    // call intersect_t on all the faces
    // return the minimum of the six
    double min_t = -1;

    for (int i = 0; i < 6; i++) {
      double t = faces[i].intersect_t(p, d);
      if (t > -EPSILON) {
        if (min_t == -1) {
          min_t = t;
        } else {
          min_t = std::min(min_t, t);
        }
      }
    }    

    return min_t;
  }

  // implement this function properly
  Vector normal(Vector p) {
    // call normal on all the faces
    // return the normal of the face that is closest to p
    double min_dist = -1;
    Vector min_normal;

    for (int i = 0; i < 6; i++) {
      Vector n = faces[i].normal(p);
      double dist = (p - ((Square)faces[i]).centroid()).length();
      if (min_dist == -1 || dist < min_dist) {
        min_dist = dist;
        min_normal = n;
      }
    }

    return min_normal;
  }

  Color getColor() {
    return Object::getColor();
  }

  Color get_color(Vector p) {
    return getColor();
  }

  void draw() {
    for (int i = 0; i < 6; i++) {
      faces[i].draw();
    }
  }
};
#endif