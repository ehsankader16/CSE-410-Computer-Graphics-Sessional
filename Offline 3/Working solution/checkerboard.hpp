#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include <cmath>
#include <string>
#include <GL/glut.h>
#include "object.hpp"
#include "bitmap_image.hpp"

class Checkerboard : public Object {
private:
  double width;
  double startX, startY;
  int n = 50;
  bool hasTexture = false;
  bitmap_image white_cell_texture;
  bitmap_image black_cell_texture;

  Color getColor(int x, int y) {

    if ((x + y) % (2 * (int)width) == 0) {
      return {1, 1, 1};
    }
    else {
      return {0, 0, 0};
    }
  }

  Color getColor(int x, int y, double cellX, double cellY) {
    // if has texture, return the color of the texture
    // else, return the color of the checkerboard  

    if (hasTexture) {
      if ((x + y) % (2 * (int)width) == 0) {
        int cx = (int)floor(cellX * white_cell_texture.width() / width);
        int cy = (int)floor(cellY * white_cell_texture.height() / width);

        rgb_t pixel = white_cell_texture.get_pixel(cx, cy);
        return {pixel.red / 255.0, pixel.green / 255.0, pixel.blue / 255.0};
      } else {
        int cx = (int)floor(cellX * black_cell_texture.width() / width);
        int cy = (int)floor(cellY * black_cell_texture.height() / width);

        rgb_t pixel = black_cell_texture.get_pixel(cx, cy);
        return {pixel.red / 255.0, pixel.green / 255.0, pixel.blue / 255.0};
      }
    }

    return getColor(x, y);
  }
public:
  Checkerboard() {
    width = 0;
  }

  Checkerboard(double width, double ambient, double diffuse, double reflection, double shininess) 
    : Object({0, 0, 0}, ambient, diffuse, reflection, 0, shininess) {
    this->width = width;
    startX = -n * width / 2;
    startY = n * width / 2;
    white_cell_texture = bitmap_image("./texture_w.bmp");
    black_cell_texture = bitmap_image("./texture_b.bmp");
  }

  double intersect_t(const Vector p, const Vector d) {
    double t = -p.z / d.z;

    if (t < EPSILON) {
      return -1;
    }
    
    return t;
  }

  void toggleHasTexture() {
    this->hasTexture = !this->hasTexture;
  }

  bool getHasTexture() {
    return this->hasTexture;
  }

  Vector normal(const Vector p) {
    return Vector(0, 0, 1);
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector p) {
    // find the closest point on the checkerboard to p
    // calculate the cell index of that point
    // if the cell index is even, return black
    // if the cell index is odd, return white

    int posX;
    int posY;
    double cellX;
    double cellY;

    posX = (int)floor(p.x);
    posY = (int)floor(p.y);

    if (p.x > 0) {
      posX = posX - (posX % (int)width);
      cellX = p.x - (int)p.x / (int)width * width;
    } else {
      posX = posX - (posX % (int)width) - (int)width;
      cellX = p.x - (int)p.x / (int)width * width + width;
    }

    if (p.y > 0) {
      posY = posY - (posY % (int)width);
      cellY = p.y - (int)p.y / (int)width * width;
    } else {
      posY = posY - (posY % (int)width)  - (int)width;
      cellY = p.y - (int)p.y / (int)width * width + width;
    }

    return getColor(posX, posY, cellX, cellY);
  }
  
  void draw() {}

  void draw(Vector position) {
    // draw a 200 x 200 checkerboard around the position
    // the position is the center of the checkerboard
    // the checkerboard is on the xz plane
    // the checkerboard is made of width X width squares
    // the color of the squares alternate between black and white
    int posX = (int)position.x;
    int posY = (int)position.z;

    posX = posX - (posX % (int)width);
    posY = posY - (posY % (int)width);

    startX = posX - n * (int)width / 2;
    startY = posY + n * (int)width / 2;

    for (int i = 0; i < n; i++) {
      int x = startX + i * (int)width;
      for (int j = 0; j < n; j++) {
        int y = startY - j * (int)width;
        Color color = getColor(x, y);
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        glVertex3f(x, y, 0);
        glVertex3f(x + width, y, 0);
        glVertex3f(x + width, y + width, 0);
        glVertex3f(x, y + width, 0);
        glEnd();
      }
    }
  }
};

#endif