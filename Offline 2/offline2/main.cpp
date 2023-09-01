#include <bits/stdc++.h>
#include "Point.hpp"
#include "Triangle.hpp"
#include "Transformation.hpp"
#include "bitmap_image.hpp"
using namespace std;

static unsigned long int g_seed = 1;
inline int g_random()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}


int main() {

    ifstream fin;
    ofstream fout;

    Point eye, look, up;
    double fovY, aspectRatio, near, far;

    //  Stage 1 Starts
    
    fin.open("../Test/1/scene.txt");
    fout.open("stage1.txt");

    fin >> eye;
    fin >> look;
    fin >> up;
    fin >> fovY >> aspectRatio >> near >> far;

    //  Creating and pushing baseline matrix in the stack
    stack<Transformation> stk;
    string command;
    int numTriangle = 0;

    Transformation tModel;
    Transformation M = tModel.identityMatrix();
    stk.push(M);

    while (true)
    {
        fin >> command;
        // fout << command << endl;
        if (command == "triangle") {
            Point p1, p2, p3;
            fin >> p1;
            fin >> p2;
            fin >> p3;

            p1 = stk.top() * p1;
            p2 = stk.top() * p2;
            p3 = stk.top() * p3;

            fout << p1 << endl;
            fout << p2 << endl;
            fout << p3 << endl;
            fout << endl;

            numTriangle++;
        } else if (command == "translate") {
            double tx, ty, tz;
            fin >> tx >> ty >> tz;

            Transformation t = tModel.translationMatrix(tx, ty, tz);
            t = stk.top() * t;

            stk.pop();
            stk.push(t);
        } else if (command == "scale") {
            double sx, sy, sz;
            fin >> sx >> sy >> sz;

            Transformation t = tModel.scalingMatrix(sx, sy, sz);
            t = stk.top() * t;

            stk.pop();
            stk.push(t);
        } else if (command == "rotate") {
            double angle, ax, ay, az;
            fin >> angle >> ax >> ay >> az;

            Transformation t = tModel.rotationMatrix(angle, ax, ay, az);
            t = stk.top() * t;
            stk.pop();
            stk.push(t);
        } else if (command == "push") {
            stk.push(stk.top());
        } else if (command == "pop") {
            if (stk.empty()) {
                cout << "Error: Stack is empty, nothing to pop" << endl;
                break;
            }
            stk.pop();
        } else if (command == "end") {
            break;
        } else {
            cout << "Command: \"" << command <<"\" is invalid" << endl;
            break;
        }
    }
    
    fin.close();
    fout.close();
    //Stage 1 Ends

    //Stage 2 Starts
    fin.open("stage1.txt");
    fout.open("stage2.txt");

    Transformation tView;
    tView = tView.identityMatrix();

    tView = tView.viewMatrix(eye, look, up);

    for (int i = 0; i < numTriangle; i++) {
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;

        p1 = tView * p1;
        p2 = tView * p2;
        p3 = tView * p3;
        
        fout << p1 << endl;
        fout << p2 << endl;
        fout << p3 << endl;
        fout << endl;
    }

    fin.close();
    fout.close();
    //Stage 2 Ends

    //Stage 3 Starts
    fin.open("stage2.txt");
    fout.open("stage3.txt");

    Transformation tProjection;
    tProjection = tProjection.identityMatrix();

    tProjection = tProjection.projectionMatrix(fovY, aspectRatio, near, far);

    for (int i = 0; i < numTriangle; i++) {
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;

        p1 = tProjection * p1;
        p2 = tProjection * p2;
        p3 = tProjection * p3;

        fout << p1 << endl;
        fout << p2 << endl;
        fout << p3 << endl;
        fout << endl;
    }

    fin.close();
    fout.close();
    //Stage 3 Ends

    // 1. Clip everything outside the viewing volume.
    // 2. Considering yourself as a parallel viewer from the XY-plane, generate the image (dimension: 
    // Screen_Width X Screen_Height) that can be seen on the XY plane after computing the necessary clipping 
    // and depth information of objects (triangles) within this viewing volume.
    // 3. Print z-buffer values into a file named z-buffer.txt . (only those values where z-buffer[row][col] < z_max)
   
    fin.open("config.txt");

    int screenWidth, screenHeight;
    fin>>screenWidth>>screenHeight;

    fin.close();

    fin.open("stage3.txt");

    vector<Triangle> triangles;
    for (int i = 0; i < numTriangle; i++) {
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        Triangle triangle(p1, p2, p3);
        int r = g_random();
        int g = g_random();
        int b = g_random();
        triangle.setColor(Color(r, g, b));
        triangles.push_back(triangle);
    }
    fin.close();

    // for (int i = 0; i < numTriangle; i++) {
    //     cout<<triangles[i].points[0]<<endl;
    //     cout<<triangles[i].points[1]<<endl;
    //     cout<<triangles[i].points[2]<<endl;
    //     cout<<endl;
    // }

    double xRightLimit = 1.0, xLeftLimit = -1.0;
    double yTopLimit = 1.0, yBottomLimit = -1.0;
    double zMax = 1.0, zFrontLimit = -1.0;

    double **zBuffer = new double*[screenHeight];
    for (int i = 0; i < screenHeight; i++) {
        zBuffer[i] = new double[screenWidth];
        for (int j = 0; j < screenWidth; j++) {
            zBuffer[i][j] = zMax;
        }
    }

    bitmap_image image(screenWidth, screenHeight);
    for(int i = 0; i < screenWidth; i++) {
        for(int j = 0; j < screenHeight; j++) {
            image.set_pixel(i,j,0,0,0);
        }
    }

    
    double dx = (xRightLimit - xLeftLimit) / screenWidth;
    double dy = (yTopLimit - yBottomLimit) / screenHeight;

    double leftX = xLeftLimit + dx / 2.0;
    double rightX = xRightLimit - dx / 2.0;
    double topY = yTopLimit - dy / 2.0;
    double bottomY = yBottomLimit + dy / 2.0;

    

    for (int i = 0; i < numTriangle; i++) {
        
        double tMinX = triangles[i].getMinX(), tMaxX = triangles[i].getMaxX();
        double tMinY = triangles[i].getMinY(), tMaxY = triangles[i].getMaxY();

        //clipping util start
        double scanMinX = max(leftX, tMinX);
        double scanMaxX = min(rightX, tMaxX);
        double scanMinY = max(bottomY, tMinY); //bottom scan line
        double scanMaxY = min(topY,tMaxY); //top scan line
        //clipping util end
        // cout<<scanMinY<<" "<<scanMaxY<<endl;
        for(double row = scanMaxY; row >= scanMinY; row -= dy) {
            vector<double>xAB = triangles[i].getRowIntersections(row);

            if(xAB[0] > xAB[1]) {
                double temp = xAB[0];
                xAB[0] = xAB[1];
                xAB[1] = temp;
            }
            
            //clipping start
            if(xAB[0] < scanMinX) {
                xAB[0] = scanMinX;
            }
            if(xAB[1] > scanMaxX) {
                xAB[1] = scanMaxX;
            }
            //clipping end

            for(double col = xAB[0]; col <= xAB[1]; col += dx) {
                double z = triangles[i].getZ(col, row);
                int colIndex = (col - xLeftLimit) / dx;
                int rowIndex = (yTopLimit - row) / dy;

                if(z < zBuffer[rowIndex][colIndex] && z > zFrontLimit) {
                    zBuffer[rowIndex][colIndex] = z;
                    image.set_pixel(colIndex, rowIndex, triangles[i].color.r, triangles[i].color.g, triangles[i].color.b);
                }
            }

        }
    }

    fout.open("z_buffer.txt");
    for(int i = 0; i < screenHeight; i++) {
        for(int j = 0; j < screenWidth; j++) {
            if(zBuffer[i][j] < zMax) {
                fout << setprecision(6) << fixed << zBuffer[i][j] << "\t";
            }
        }
        fout<<endl;
    }
    fout.close();

    image.save_image("out.bmp");

    for (int i = 0; i < screenHeight; i++) {
        delete[] zBuffer[i];
    }
    delete[] zBuffer;

    return 0;
}