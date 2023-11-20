#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <iostream>

class Vector {
public:
  double x, y, z;

  Vector();
  Vector(double x, double y, double z);
  Vector operator+(const Vector& other);
  Vector operator*(const Vector& other);
  Vector operator*(const double& other);
  Vector operator/(const Vector& other);
  Vector operator/(const double& other);

  friend Vector operator-(const Vector &v1, const Vector &v2);
  friend Vector operator*(double s, const Vector &v);
  friend Vector operator/(double &s, const Vector &v);
  friend std::ostream &operator<<(std::ostream &out, const Vector &v);

  double dot(const Vector& other) const;
  Vector cross(const Vector& other) const;
  Vector rotate(const Vector& axis, double angle) const;
  double length() const;
  Vector normalize() const;

};

Vector::Vector() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

Vector::Vector(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector Vector::operator+(const Vector& other) {
  Vector result;
  result.x = this->x + other.x;
  result.y = this->y + other.y;
  result.z = this->z + other.z;
  return result;
}

Vector operator-(const Vector& v1, const Vector& v2) {
  Vector result;
  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  result.z = v1.z - v2.z;
  return result;
}

Vector Vector::operator*(const Vector& other) {
  Vector result;
  result.x = this->x * other.x;
  result.y = this->y * other.y;
  result.z = this->z * other.z;
  return result;
}

Vector Vector::operator/(const Vector& other) {
  Vector result;
  result.x = this->x / other.x;
  result.y = this->y / other.y;
  result.z = this->z / other.z;
  return result;
}

Vector Vector::operator*(const double& other) {
  Vector result;
  result.x = this->x * other;
  result.y = this->y * other;
  result.z = this->z * other;
  return result;
}

Vector Vector::operator/(const double& other) {
  Vector result;
  result.x = this->x / other;
  result.y = this->y / other;
  result.z = this->z / other;
  return result;
}

Vector operator*(double s, const Vector &v) {
  Vector result;
  result.x = s * v.x;
  result.y = s * v.y;
  result.z = s * v.z;
  return result;
}

Vector operator/(double &s, const Vector &v) {
  Vector result;
  result.x = s / v.x;
  result.y = s / v.y;
  result.z = s / v.z;
  return result;
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return out;
}

double Vector::dot(const Vector& other) const {
  return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector Vector::cross(const Vector& other) const {
  Vector result;
  result.x = this->y * other.z - this->z * other.y;
  result.y = this->z * other.x - this->x * other.z;
  result.z = this->x * other.y - this->y * other.x;
  return result;
}

Vector Vector::rotate(const Vector& axis, double angle) const {
  // rodrigues rotation
  Vector result;
  Vector axis_norm = axis.normalize();
  angle = angle * M_PI / 180;
  result = cos(angle) * *this + axis_norm.cross(*this) 
    * sin(angle) + axis_norm * axis_norm.dot(*this) * (1 - cos(angle));

  return result;
}

double Vector::length() const {
  return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector Vector::normalize() const {
  Vector result;
  double length = this->length();
  result.x = this->x / length;
  result.y = this->y / length;
  result.z = this->z / length;
  return result;
}

#endif