#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include <bits/stdc++.h>

#define epsilon 0.000001
using namespace std;

class Object;
class Light;
class SpotLight;
extern vector<Object*> objects;
extern vector<Light> lights;
extern vector<SpotLight> spotLights;

bool flag = false;

class Color
{
    public:
    double r, g, b;
    Color() {
        r = 1.0;
        g = 1.0;
        b = 1.0;
    }

    Color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void adjust() {
        if(this->r < 0)
            this->r = 0;
        if(this->g < 0)
            this->g = 0;
        if(this->b < 0)
            this->b = 0;

        if(this->r > 1)
            this->r = 1;
        if(this->g > 1)
            this->g = 1;
        if(this->b > 1)
            this->b = 1;
    }

    Color operator+(double value) {
        return Color(this->r + value, this->g + value, this->b + value);
    }

    Color operator+(Color color) {
        return Color(this->r + color.r, this->g + color.g, this->b + color.b);
    }

    Color operator*(double value) {
        return Color(this->r * value, this->g * value, this->b * value);
    }

    Color operator*(Color color) {
        return Color(this->r * color.r, this->g * color.g, this->b * color.b);
    }
};


class Vector
{
    public:
    double x, y, z, w;

    Vector() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1;
    }
    
    Vector(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }

    Vector(double x, double y, double z, double w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector(const Vector &p) {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        this->w = p.w;  
    }

    double getLength() {
        return sqrt(x*x+y*y+z*z);
    }
    void normalize() {
        double length = getLength();
        if (length == 0) {
            std::cout << "Error: length is zero, can't divide by zero" << endl;
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

    Vector operator+(Vector p) {
        return Vector(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Vector operator-(Vector p) {
        return Vector(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Vector operator+(double value) {
        return Vector(this->x + value, this->y + value, this->z + value);
    }

    Vector operator*(double value) {
        return Vector(this->x * value, this->y * value, this->z * value);
    }

    double operator*(Vector p) {
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }

    Vector operator^ (Vector p) {
        return Vector(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
    }

    friend ostream& operator<< (ostream& fout, Vector &p) {
        fout << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return fout;
    }

    friend ifstream& operator>> (ifstream& fin, Vector &p) {
        fin >> p.x >> p.y >> p.z;
        return fin;
    }

};

//write a object class to store the object information that will store 
//reference point, color, normal, ambient, diffuse, specular and reflection coefficient
class Light
{
    public:
    Vector position;
    double falloff;
    Color color;
    Light() {}
    Light(Vector position, double falloff) {
        this->position = position;
        this->falloff = falloff;
        this->color = Color(1, 1, 1);
    }

    void draw() {

    }

};

class SpotLight
{
    public:
    Vector position;
    double falloff;
    Vector lookDir;
    double cutoffAngle;
    Color color;

    SpotLight() {}
    SpotLight(Vector position, double falloff, Vector lookDir, double cutoffAngle) {
        this->position = position;
        this->falloff = falloff;
        this->lookDir = lookDir;
        this->cutoffAngle = cutoffAngle;
        this->color = Color(1, 1, 1);
    }

    void draw() {
       
    }

};
class Ray
{
    public:
    Vector origin, dir;
    Ray() {}
    Ray(Vector origin, Vector dir) {
        this->origin = origin;
        this->dir = dir;
        this->dir.normalize();
    }
};


double calculateDeterminant(double matrix[3][3]) {
    double d1 = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
	double d2 = matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
	double d3 = matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
	return d1 - d2 + d3;
}

class Object
{
    public:
    Vector referencePoint;
    double length;
    Color color;
    double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
    int shininess;

    Object() {}
    virtual void draw() {}

    void setReferencePoint(Vector referencePoint) {
        this->referencePoint = referencePoint;
    }

    void setColor(Color color) {
        this->color = color;
    }

    void setCoefficients(double ambient, double diffuse, double specular, double reflection) {
        this->ambientCoeff = ambient;
        this->diffuseCoeff = diffuse;
        this->specularCoeff = specular;
        this->reflectionCoeff = reflection;
    }

    void setShininess(int shininess) {
        this->shininess = shininess;
    }
    Color getColor() {
        return color;
    }

    virtual Color getColorAtPoint(Vector point) {
        return color;
    }

    virtual double getParamT(Ray& ray) {
        // std::cout<<"In object class paramT"<<endl;
        return -1;
    }
    
    double intersect(Ray& ray, Color& pixelColor, int currentLevel, int recursionLevel) {
        double paramT = this->getParamT(ray);
        //cout<<"recursion level"<<recursionLevel<<endl;
        if(paramT < 0) 
            return -1;
        if(recursionLevel == 0) 
            return paramT;
        Vector intersectionPoint = ray.origin + ray.dir * paramT;
        Color colorAtIntersection = getColorAtPoint(intersectionPoint);

        double lambert = 0, phong = 0;
        for(int i = 0; i < lights.size(); i++) {
            Vector lightPos = lights[i].position;
            Vector lightDir = lightPos - intersectionPoint;
            lightDir.normalize();
            
            double disToLightSource = (intersectionPoint - lightPos).getLength();
            
            Ray lightRay(intersectionPoint + epsilon, lightDir);
            bool illuminate = true;
            for(int j = 0; j < objects.size(); j++) {
                double otherParamT = objects[j]->getParamT(lightRay);
                if(otherParamT > epsilon && otherParamT < disToLightSource - epsilon) {
                    illuminate = false;
                    break;
                }
            }
            
            if(illuminate) {
                Vector normalAtIntersection = getNormalAtPoint(intersectionPoint, lightRay);
                normalAtIntersection.normalize();
                double scalingFactor = exp(-1.0 * disToLightSource * disToLightSource * lights[i].falloff);
                lambert += max(0.0, (lightDir * normalAtIntersection)) * scalingFactor;
                Ray reflectedLightRay(intersectionPoint, lightRay.dir - 
                            normalAtIntersection * 2 * (lightRay.dir * normalAtIntersection));
                double phongBase = max(0.0, ray.dir * reflectedLightRay.dir);
                phong += pow(phongBase, shininess) * scalingFactor;
            }

        }
        Color lightColor(1,1,1);
        pixelColor = colorAtIntersection * (lightColor * (lambert * diffuseCoeff) + ambientCoeff) + (lightColor * phong * specularCoeff);

        lambert = 0, phong = 0;
        for(int i = 0; i < spotLights.size(); i++) {
            Vector spotLightPos = spotLights[i].position;
            Vector spotLightDir = spotLightPos - intersectionPoint;
            spotLightDir.normalize();
            
            double disToLightSource = (intersectionPoint - spotLightPos).getLength();
            
            double angle = acos(spotLightDir * spotLights[i].lookDir);

            if(angle > spotLights[i].cutoffAngle - epsilon) {
                continue;
            }

            Ray spotLightRay(intersectionPoint + epsilon, spotLightDir);
            bool illuminate = true;
            for(int j = 0; j < objects.size(); j++) {
                double otherParamT = objects[j]->getParamT(spotLightRay);
                if(otherParamT > epsilon && otherParamT < disToLightSource - epsilon) {
                    illuminate = false;
                    break;
                }
            }
            
            if(illuminate) {
                Vector normalAtIntersection = getNormalAtPoint(intersectionPoint, spotLightRay);
                normalAtIntersection.normalize();
                double scalingFactor = exp(-1.0 * disToLightSource * disToLightSource * spotLights[i].falloff);
                lambert += max(0.0, (spotLightDir * normalAtIntersection )) * scalingFactor;
                Ray reflectedSpotLightRay(intersectionPoint, spotLightRay.dir - 
                            normalAtIntersection * 2 * (spotLightRay.dir * normalAtIntersection));
                double phongBase = max(0.0, ray.dir * reflectedSpotLightRay.dir);
                phong += pow(phongBase, shininess) * scalingFactor;
            }
        }
        pixelColor = pixelColor + colorAtIntersection * (lightColor * (lambert * diffuseCoeff) + ambientCoeff) + (lightColor * phong * specularCoeff);

        //reflection code
        if(currentLevel < recursionLevel) {
            Vector normalAtIntersection = getNormalAtPoint(intersectionPoint, ray);
            Ray reflectedRay(intersectionPoint + epsilon, ray.dir - 
                        normalAtIntersection * 2 * (ray.dir * normalAtIntersection));
            int nearestObjectIndex = -1;
            double reflParamTMin = -1;
                
            for(int k = 0; k < objects.size(); k++) {
                double reflParamT = objects[k]->getParamT(reflectedRay);
                if(reflParamT > -epsilon && (nearestObjectIndex == -1 || reflParamT < reflParamTMin - epsilon)) {
                    reflParamTMin = reflParamT;
                    nearestObjectIndex = k;
                }
            }

            if(nearestObjectIndex != -1) {
                Color tempColor(0,0,0);
                Vector reflIntersectionPoint = reflectedRay.origin + reflectedRay.dir * reflParamTMin;
                double t = objects[nearestObjectIndex]->intersect(reflectedRay, tempColor, currentLevel + 1, recursionLevel);
                pixelColor = pixelColor + tempColor * reflectionCoeff;
            }
        }
        return paramT;
    }

    virtual Vector getNormalAtPoint(Vector point, Ray ray) {
        return Vector();
    }

    virtual Vector getColor(Vector point) {
        return Vector();
    }

};

class CheckerBoard : public Object
{
    public:
    double numTiles, tileWidth;
    CheckerBoard(){}
    CheckerBoard(double tileWidth) {
        this->referencePoint = Vector(0, 0, 0);
        this->numTiles = 500;
        this->tileWidth = tileWidth;
        this->length = tileWidth;
    }
  
    double getParamT(Ray& ray) {
        Vector normal(0, 0, 1);
        double intersectionCheck = normal * ray.dir;
        if(intersectionCheck < epsilon && intersectionCheck > -epsilon) {
            return -1;
        }
		double paramT = -ray.origin.z / ray.dir.z;
        if (paramT < epsilon) 
            return -1;
        return paramT;
    }

    Vector getNormalAtPoint(Vector point, Ray ray) {
        Vector normal(0, 0, 1);
        double rayProjection = ray.dir * normal;
        if(rayProjection < 0){
            return (normal * (-1.0));
        }
        return normal;
    }

    void draw() {
        for(int i = -numTiles; i < numTiles; i++) {
            for(int j = -numTiles; j < numTiles; j++) {
                if((i + j) & 1) {
                    glColor3f(0, 0, 0);
                } else {
                    glColor3f(1, 1, 1);
                }
                glBegin(GL_QUADS);
                    glVertex3f(referencePoint.x + i * tileWidth, referencePoint.y + j * tileWidth, referencePoint.z);
                    glVertex3f(referencePoint.x + (i + 1) * tileWidth, referencePoint.y + j * tileWidth, referencePoint.z);
                    glVertex3f(referencePoint.x + (i + 1) * tileWidth, referencePoint.y + (j + 1) * tileWidth, referencePoint.z);
                    glVertex3f(referencePoint.x + i * tileWidth, referencePoint.y  + (j + 1) * tileWidth, referencePoint.z);
                glEnd();
            }
        }
    }

    Color getColorAtPoint(Vector point) {
        int i = floor((point.x - referencePoint.x) / tileWidth);
        int j = floor((point.y - referencePoint.y) / tileWidth);
        if((i + j) & 1) {
            return Color(0, 0, 0);
        } else {
            return Color(1, 1, 1);
        }
    }
};

class Sphere : public Object
{
    public:
    double radius;

    Sphere() {}
    Sphere(Vector center, double radius) {
        this->referencePoint = center;
        this->radius = radius;
    }

    double getParamT(Ray& ray) {
        Vector rO = ray.origin;
        rO = rO - referencePoint;
        Vector rD = ray.dir;
        double a = 1.0;
        double b = 2.0 * (rD * rO);
        double c = (rO * rO) - (radius * radius);
        double determinant = b * b -  4 * c;
        if(determinant < 0) {
            return -1;
        }

        double t1 = (-b + sqrt(determinant)) / (2 * a);
        double t2 = (-b - sqrt(determinant)) / (2 * a);
        if(t1 > -epsilon) {
            return t1;
        } else if(t2 > -epsilon) {
            return t2;
        }
        return -1;
    }
    
    Vector getNormalAtPoint(Vector point, Ray ray) {
        Vector normal = point - referencePoint;
        double distance = (ray.origin - referencePoint).getLength(); // distance from the center of the sphere
        // if distance is less than radius, then the point is inside the sphere
        if (distance < radius) {
            normal = normal * (-1.0);
        }
        normal.normalize();
        return normal;
    }

    void draw() {
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);
            glutSolidSphere(radius, 100, 100);
        glPopMatrix();
    }
};

//write class to store triangle information
class Triangle : public Object
{
    public:
    Vector a, b, c;

    Triangle() {}
    Triangle(Vector a, Vector b, Vector c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void draw() {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        glEnd();
    }

    double getParamT(Ray& ray) {

        double AMatrix[3][3] = {
            {a.x - b.x, a.x - c.x, ray.dir.x},
            {a.y - b.y, a.y - c.y, ray.dir.y},
            {a.z - b.z, a.z - c.z, ray.dir.z}
        };

        double betaMatrix[3][3] = {
            {a.x - ray.origin.x, a.x - c.x, ray.dir.x},
            {a.y - ray.origin.y, a.y - c.y, ray.dir.y},
            {a.z - ray.origin.z, a.z - c.z, ray.dir.z}
        };

        double gammaMatrix[3][3] = {
            {a.x - b.x, a.x - ray.origin.x, ray.dir.x},
            {a.y - b.y, a.y - ray.origin.y, ray.dir.y},
            {a.z - b.z, a.z - ray.origin.z, ray.dir.z}
        };
        
        double tMatrix[3][3] = {
            {a.x - b.x, a.x - c.x, a.x - ray.origin.x},
            {a.y - b.y, a.y - c.y, a.y - ray.origin.y},
            {a.z - b.z, a.z - c.z, a.z - ray.origin.z}
        };

        double detA = calculateDeterminant(AMatrix);
        double detBeta = calculateDeterminant(betaMatrix);
        double detGamma = calculateDeterminant(gammaMatrix);
        double detT = calculateDeterminant(tMatrix);
        
        double beta = detBeta / detA;
        double gamma = detGamma / detA;
        double t = detT / detA;

        if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0) {
            return t;
        } else { 
            return -1;
        }
    }

    Vector getNormalAtPoint(Vector point, Ray ray) {
        Vector normal = (b - a) ^ (c - a);
        normal.normalize();
        double rayProjection = ray.dir * normal;
        if(rayProjection < 0){
            return (normal * (-1.0));
        }
        return normal;
    }

    Vector getColor(Vector intersectionPoint) {
        return Vector(color.r, color.g, color.b);
    }

};

vector<Triangle*> getSquareTriangles(Vector a, Vector b, Vector c, Vector d) {
    vector<Triangle*> triangles;
    triangles.push_back(new Triangle(a, b, c));
    triangles.push_back(new Triangle(a, d, c));
    return triangles;
}

vector<Triangle*> getCubeTriangles(Vector bottomLowerLeft, double side) {
    vector<Vector> vertices;
    vertices.push_back(bottomLowerLeft);
    vertices.push_back(bottomLowerLeft + Vector(side, 0, 0));
    vertices.push_back(bottomLowerLeft + Vector(side, side, 0));
    vertices.push_back(bottomLowerLeft + Vector(0, side, 0));
        
    vertices.push_back(bottomLowerLeft + Vector(0, 0, side));
    vertices.push_back(bottomLowerLeft + Vector(side, 0, side));
    vertices.push_back(bottomLowerLeft + Vector(side, side, side));
    vertices.push_back(bottomLowerLeft + Vector(0, side, side));

    vector<Triangle*> squareTriangles;
    vector<Triangle*> cubeTriangles;

    squareTriangles = getSquareTriangles(vertices[0], vertices[1], vertices[2], vertices[3]); //bottom face
    cubeTriangles.insert(cubeTriangles.end(), squareTriangles.begin(), squareTriangles.end());

    squareTriangles = getSquareTriangles(vertices[4], vertices[5], vertices[6], vertices[7]); //top face
    cubeTriangles.insert(cubeTriangles.end(), squareTriangles.begin(), squareTriangles.end());

    squareTriangles = getSquareTriangles(vertices[0], vertices[1], vertices[5], vertices[4]); //back face
    cubeTriangles.insert(cubeTriangles.end(), squareTriangles.begin(), squareTriangles.end());

    squareTriangles = getSquareTriangles(vertices[3], vertices[2], vertices[6], vertices[7]); //front face
    cubeTriangles.insert(cubeTriangles.end(), squareTriangles.begin(), squareTriangles.end());

    squareTriangles = getSquareTriangles(vertices[0], vertices[3], vertices[7], vertices[4]); //left face
    cubeTriangles.insert(cubeTriangles.end(), squareTriangles.begin(), squareTriangles.end());

    squareTriangles = getSquareTriangles(vertices[1], vertices[2], vertices[6], vertices[5]); //right face
    cubeTriangles.insert(cubeTriangles.end(), squareTriangles.begin(), squareTriangles.end());

    return cubeTriangles;
}

vector<Triangle*> getPyramidTriangles(Vector lowestPoint, double width, double height) {
    vector<Vector> vertices;
    vertices.push_back(lowestPoint);
    vertices.push_back(lowestPoint + Vector(width, 0, 0));
    vertices.push_back(lowestPoint + Vector(width, width, 0));
    vertices.push_back(lowestPoint + Vector(0, width, 0));
    vertices.push_back(lowestPoint + Vector(width/2.0, width/2.0, height));

    vector<Triangle*> squareTriangles;
    vector<Triangle*> pyramidTriangles;

    squareTriangles = getSquareTriangles(vertices[0], vertices[1], vertices[2], vertices[3]);
    pyramidTriangles.insert(pyramidTriangles.end(), squareTriangles.begin(), squareTriangles.end());
    pyramidTriangles.push_back(new Triangle(vertices[0], vertices[1], vertices[4]));
    pyramidTriangles.push_back(new Triangle(vertices[1], vertices[2], vertices[4]));
    pyramidTriangles.push_back(new Triangle(vertices[2], vertices[3], vertices[4]));
    pyramidTriangles.push_back(new Triangle(vertices[3], vertices[0], vertices[4]));

    return pyramidTriangles;
}



