#include "Point.hpp"
#include <map>
#include <vector>

using namespace std;

#ifndef TRIANGLE_H
#define TRIANGLE_H



class Color
{
    public:
    int r, g, b;
    Color() {
        r = 0;
        g = 0;
        b = 0;
    }

    Color(int r, int g, int b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Triangle
{
    public:
    Point points[3];
    Color color;

    Triangle(Point a, Point b, Point c) {
        points[0] = a;
        points[1] = b;
        points[2] = c;
    }

    void setColor(Color color) {
        this->color = color;
    }

    double getMinX() {
        double minX = min(points[0].x, points[1].x);
        minX = min(minX, points[2].x);
        return minX;
    }

    double getMaxX() {
        double maxX = max(points[0].x, points[1].x);
        maxX = max(maxX, points[2].x);
        return maxX;
    }

    double getMinY() {
        double minY = min(points[0].y, points[1].y);
        minY = min(minY, points[2].y);
        return minY;
    }

    double getMaxY() {
        double maxY = max(points[0].y, points[1].y);
        maxY = max(maxY, points[2].y);
        return maxY;
    }

    double getZ(double x, double y) {
        //find normal of this triangle's plane
        Point a = points[1] - points[0];
        Point b = points[2] - points[0];
        Point normal = a ^ b;
        normal.normalize();

        //find d
        double d = -(normal.x * points[0].x + normal.y * points[0].y + normal.z * points[0].z);
        double z = -(normal.x * x + normal.y * y + d) / normal.z;
        return z;
    }

    vector<double> getRowIntersections(double row) {

        vector<double>xAB(2);
        int pointCount = 0;
        for(int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            if(points[i].y == points[j].y)
                continue;
            if(row >= min(points[i].y, points[j].y) && row <= max(points[i].y, points[j].y)) {
                xAB[pointCount] = points[i].x + (row - points[i].y) * (points[i].x - points[j].x)/(points[i].y - points[j].y);
                pointCount++;
            }    
        }
        return xAB;
    }

};

#endif