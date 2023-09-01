#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

#ifndef POINT_H
#define POINT_H

class Point
{
    public:
    double x, y, z, w;

    Point() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1;
    }
    
    Point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }

    Point(double x, double y, double z, double w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Point(const Point &p) {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        this->w = p.w;  
    }

    void normalize() {
        double length = sqrt(x * x + y * y + z * z);
        if (length == 0) {
            cout << "Error: length is zero, can't divide by zero" << endl;
            return;
        }
        this->x /= length;
        this->y /= length;
        this->z /= length;
    }

    void scaleToOne() {
        x /= w;
        y /= w;
        z /= w;
        w /= w;
    }

    Point operator+(Point p) {
        return Point(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Point operator-(Point p) {
        return Point(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Point operator*(double value) {
        return Point(this->x * value, this->y * value, this->z * value);
    }

    double operator*(Point p) {
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }

    Point operator^ (Point p) {
        return Point(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
    }

    friend ostream& operator<< (ostream& fout, Point &p) {
        fout << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return fout;
    }

    friend ifstream& operator>> (ifstream& fin, Point &p)
    {
        fin >> p.x >> p.y >> p.z;
        return fin;
    }

};

#endif