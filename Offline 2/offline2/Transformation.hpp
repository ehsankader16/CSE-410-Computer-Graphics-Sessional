#include <bits/stdc++.h>
#include "Point.hpp"

using namespace std;

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

class Transformation
{
    public:
    double matrix[4][4];
    Transformation() {
        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                matrix[i][j] = 0.0;
            }
        }
    }

    Transformation(const Transformation &c) {
        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                this->matrix[i][j] = c.matrix[i][j];
            }
        }
    }

    Transformation identityMatrix() {
        Transformation t;
        for (int i = 0; i < 4; i++) {
            t.matrix[i][i] = 1.0;
        }
        return t;
    }

    Point operator*(Point p) {
        double result[4];
        double pointVec[4] = {p.x, p.y, p.z, p.w};

        for(int i = 0; i < 4; i++) {
            result[i] = 0;
            for(int j = 0; j < 4; j++) {
                result[i] += this->matrix[i][j] * pointVec[j];
            }
        }
        Point point(result[0], result[1], result[2], result[3]);
        point.scaleToOne();
        return point;
    }

    Transformation operator*(Transformation t) {
        Transformation result;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) { 
                result.matrix[i][j] = 0;
                for(int k = 0; k < 4; k++) {
                    result.matrix[i][j] += this->matrix[i][k] * t.matrix[k][j];
                }
            }
        }
        return result;
    }

    Transformation translationMatrix(double tx, double ty, double tz) {
        Transformation tTranslation = identityMatrix();
        tTranslation.matrix[0][3] = tx;
        tTranslation.matrix[1][3] = ty;
        tTranslation.matrix[2][3] = tz;
        return tTranslation;
    }

    Transformation scalingMatrix(double sx, double sy, double sz) {
        Transformation tScale = identityMatrix();
        tScale.matrix[0][0] = sx;
        tScale.matrix[1][1] = sy;
        tScale.matrix[2][2] = sz;
        return tScale;
    }

    Point rodriguesFormula(Point x, Point a, double theta) {
        double thetaRad = theta * acos(-1) / 180.0;
        double cosTheta = cos(thetaRad);
        double sinTheta = sin(thetaRad);
        Point result = x * cosTheta + a * (a * x) * (1 - cosTheta) + (a ^ x) * sinTheta;
        return result;
    }

    
    Transformation rotationMatrix(double theta, double ax, double ay, double az) {
        Transformation tRotation = identityMatrix();

        Point a(ax, ay, az);
        a.normalize();

        Point c1 = rodriguesFormula(Point(1, 0, 0), a, theta);
        Point c2 = rodriguesFormula(Point(0, 1, 0), a, theta);
        Point c3 = rodriguesFormula(Point(0, 0, 1), a, theta);

        tRotation.matrix[0][0] = c1.x;
        tRotation.matrix[1][0] = c1.y;
        tRotation.matrix[2][0] = c1.z;

        tRotation.matrix[0][1] = c2.x;
        tRotation.matrix[1][1] = c2.y;
        tRotation.matrix[2][1] = c2.z;

        tRotation.matrix[0][2] = c3.x;
        tRotation.matrix[1][2] = c3.y;
        tRotation.matrix[2][2] = c3.z;
        
        return tRotation;
    }

    Transformation viewMatrix(Point eye, Point look, Point up) {

        Point l = look - eye;
        l.normalize();
        Point r = l ^ up;
        r.normalize();
        Point u = r ^ l;
        u.normalize();

        Transformation tView = translationMatrix(-eye.x, -eye.y, -eye.z);

        this->matrix[0][0] = r.x;
        this->matrix[0][1] = r.y;
        this->matrix[0][2] = r.z;
        
        this->matrix[1][0] = u.x;
        this->matrix[1][1] = u.y;
        this->matrix[1][2] = u.z;
        
        this->matrix[2][0] = -l.x;
        this->matrix[2][1] = -l.y;
        this->matrix[2][2] = -l.z;

        return (*this) * tView;
    }

    //write code for  projectionMatrix
    Transformation projectionMatrix(double fovY, double aspectRatio, double near, double far) {
        Transformation tProjection = identityMatrix();

        double fovYRad = (fovY / 2.0) * acos(-1) / 180.0;

        double fovX = fovY * aspectRatio;
        double fovXRad = (fovX / 2.0) * acos(-1) / 180.0;

        double t = tan(fovXRad) * near;
        double r = tan(fovYRad) * near;
       

        tProjection.matrix[0][0] = near / r;
        tProjection.matrix[1][1] = near / t;
        tProjection.matrix[2][2] = -(far + near) / (far - near);
        tProjection.matrix[2][3] = -(2.0 * far * near) / (far - near);
        tProjection.matrix[3][2] = -1.0;
        tProjection.matrix[3][3] = 0.0;

        return tProjection;
    }

};

#endif