#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include <bits/stdc++.h>
#include "1805067_bitmap_image.hpp"
#include "1805067_util.hpp"


#define windowWidth  600
#define windowHeight 600
#define PI acos(-1.0)

double nearDistance, farDistance, fovY, aspectRatio;
int recursionLevel, numWidthPixels, numHeightPixels;
int numObjects;
int numLights;
int numSpotLights;
bool textureMode = false;

vector<Object*> objects;
vector<Light> lights;
vector<SpotLight> spotLights;

Vector camPos(0, -300, 50);
Vector camLook(0, 1, 0);
Vector camRight(1, 0, 0);
Vector camUp(0, 0, 1);

void drawAxes() {
    glLineWidth(5);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0, 0, 0);
        glVertex3f(100, 0, 0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0.0, 0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0.0, 0.0, 0);
        glVertex3f(0.0, 0.0, 100.0);
    glEnd();
}

void loadData() {
    ifstream fin("1805067_description.txt");

    fin >> nearDistance >> farDistance >> fovY >> aspectRatio;
    fin >> recursionLevel;
    fin >> numWidthPixels;
    numHeightPixels = numWidthPixels;
     
    
    double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
    int shininess;

    int checkerBoardTileSize;
    fin >> checkerBoardTileSize;
    fin >> ambientCoeff >> diffuseCoeff >> reflectionCoeff;
    
    Object *checkerBoard = new CheckerBoard(checkerBoardTileSize);
    checkerBoard->setCoefficients(ambientCoeff, diffuseCoeff, 0.0, reflectionCoeff);
    checkerBoard->setTextureMode(textureMode);
    objects.push_back(checkerBoard);

    fin >> numObjects;

    for(int i = 0; i < numObjects; i++) {
        string objectType;
        fin >> objectType;

        if(objectType == "sphere") {
            Vector center;
            double radius;
            Color color;
            fin >> center;
            fin >> radius;
            fin >> color.r >> color.g >> color.b;
            fin >> ambientCoeff >> diffuseCoeff >> specularCoeff >> reflectionCoeff;
            fin >> shininess;
            Object *sphere = new Sphere(center, radius);
            sphere->setColor(color);
            sphere->setCoefficients(ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff);
            sphere->setShininess((int)shininess);
            objects.push_back(sphere);
        } else if(objectType == "cube") {
            Vector bottomLowerLeft;
            double side;
            Color color;
            fin >> bottomLowerLeft;
            fin >> side;
            fin >> color.r >> color.g >> color.b;
            fin >> ambientCoeff >> diffuseCoeff >> specularCoeff >> reflectionCoeff;
            fin >> shininess;
            vector<Triangle*> cubeTriangles = getCubeTriangles(bottomLowerLeft, side);
            for(int i = 0; i < cubeTriangles.size(); i++) {
                cubeTriangles[i]->setColor(color);
                cubeTriangles[i]->setCoefficients(ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff);
                cubeTriangles[i]->setShininess(shininess);
                objects.push_back(cubeTriangles[i]);
            }
        } else if(objectType == "pyramid") {
            Vector lowestPoint;
            double width, height;
            Color color;
            fin >> lowestPoint;
            fin >> width >> height;
            fin >> color.r >> color.g >> color.b;
            fin >> ambientCoeff >> diffuseCoeff >> specularCoeff >> reflectionCoeff;
            fin >> shininess;
            vector<Triangle*> pyramidTriangles = getPyramidTriangles(lowestPoint, width, height);
            for(int i = 0; i < pyramidTriangles.size(); i++) {
                pyramidTriangles[i]->setColor(color);
                pyramidTriangles[i]->setCoefficients(ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff);
                pyramidTriangles[i]->setShininess(shininess);
                objects.push_back(pyramidTriangles[i]);
            }
        }
    }

    fin >> numLights;

    for(int i = 0; i < numLights; i++) {
        Vector position;
        double falloff;
		fin >> position;
        fin >> falloff;
        Light light(position, falloff);
		lights.push_back(light);
    }

    fin >> numSpotLights;

    for(int i = 0; i < numSpotLights; i++) {
        Vector position, lookDirection;
        double falloff, cutoffAngle;
        fin >> position;
        fin >> falloff;
        fin >> lookDirection;
        fin >> cutoffAngle;
        SpotLight spotLight(position, falloff, lookDirection, cutoffAngle);
        spotLights.push_back(spotLight);
    }
    
}

void checkData()
{
    for(int i = 0; i < objects.size(); i++) {
        Object *object = objects[i];
        std::cout << "ref_point"<< object->referencePoint << endl;
        std::cout << "color:"<<object->color.r << " " << object->color.g << " " << object->color.b << endl;
        std::cout << " l:" << object->length << endl;
        std::cout << "ambient:"<<object->ambientCoeff << " diffuse:" << object->diffuseCoeff << " specular" << object->specularCoeff << " reflection" << object->reflectionCoeff << endl;
        std::cout << "shininess:"<<object->shininess << endl;
    }
}
/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST); 
}

void capture() {
    cout << "capture started"<<endl;
    bitmap_image image(numWidthPixels, numHeightPixels);
	for(int i = 0; i < numWidthPixels; i++) {
		for(int j = 0; j < numHeightPixels; j++) {
            image.set_pixel(i, j, 0, 0, 0);
        }
	}

    double fovYRad = fovY * PI / 180.0;
    double height = 2 * nearDistance * tan(fovYRad/2);
    double fovXRad = aspectRatio * fovYRad;
    double width = 2 * nearDistance * tan(fovXRad/2);

    double dx = (1.0 * width) / numWidthPixels;
    double dy = (1.0 * height) / numHeightPixels;

    Vector tempCamLook = camLook;
    tempCamLook.normalize();
    Vector topLeftPixel = camPos + tempCamLook * nearDistance - camRight * (width / 2.0) + camUp * (height / 2.0);
   
    int nearestObjectIndex;
    double paramT, paramTMin;
    
    objects[0]->setTextureMode(textureMode);

    Vector currPixel;
    for(int i = 0; i < numWidthPixels; i++) {
		for(int j = 0; j < numHeightPixels; j++) {
            currPixel = topLeftPixel + (camRight * i * dx) - (camUp * j * dy);
            Ray ray(currPixel, currPixel - camPos);

            nearestObjectIndex = -1;
            paramTMin = -1;
            
            for(int k = 0; k < objects.size(); k++) {
                double paramT = objects[k]->getParamT(ray);
                if(paramT > epsilon && (nearestObjectIndex == -1 || paramT < paramTMin - epsilon)) {
                    paramTMin = paramT;
                    nearestObjectIndex = k;
                }
            }

            if(nearestObjectIndex != -1) {
                Vector intersectionPoint = ray.origin + ray.dir * paramTMin;
                Color pixelColor = objects[nearestObjectIndex]->getColorAtPoint(intersectionPoint);
				double t = objects[nearestObjectIndex]->intersect(ray, pixelColor, recursionLevel);
                pixelColor.adjust();
                image.set_pixel(i, j, 255.0 * pixelColor.r, 255 * pixelColor.g, 255 * pixelColor.b);
            }
        }
	}
    image.save_image("raytracing.bmp");
    std::cout<<"leaving capture function"<<endl;
}
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    gluLookAt(camPos.x,camPos.y,camPos.z,
              camPos.x+camLook.x,camPos.y+camLook.y,camPos.z+camLook.z,
              camUp.x,camUp.y,camUp.z);
    
    //drawAxes();
        // cout << "numberOfObjects" << objects.size() << endl;
    for (int i = 0; i < objects.size(); i++){
        glPushMatrix();
            Object *object = objects[i];
            object->draw();
        glPopMatrix();
    }
    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(fovY, aspectRatio, nearDistance, farDistance);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    double rate = M_PI / 12.0;
    Vector tempCamLook = camLook;
    tempCamLook.normalize();
    Vector tempCamUp = camUp;
    tempCamUp.normalize();
    Vector tempCamRight = camRight;
    tempCamRight.normalize();
    switch (key) {
        case '0':
			capture();
            std::cout<<"capture break"<<endl;
			break;
        case '1':
            camRight = (tempCamRight * cos(rate)) + (tempCamLook * sin(rate));
            // camRight.x = (camRight.x * cos(rate)) + (camLook.x * sin(rate));
            // camRight.y = (camRight.y * cos(rate)) + (camLook.y * sin(rate));
            // camRight.z = (camRight.z * cos(rate)) + (camLook.z * sin(rate));

            camLook = (tempCamLook * cos(rate)) - (tempCamRight * sin(rate));
			// camLook.x = (camLook.x * cos(rate)) - (camRight.x * sin(rate));
            // camLook.y = (camLook.y * cos(rate)) - (camRight.y * sin(rate));
            // camLook.z = (camLook.z * cos(rate)) - (camRight.z * sin(rate));
            
			break;

        case '2':
            camRight = (tempCamRight * cos(-rate)) + (tempCamLook * sin(-rate));
			// camRight.x = (camRight.x * cos(-rate)) + (camLook.x * sin(-rate));
            // camRight.y = (camRight.y * cos(-rate)) + (camLook.y * sin(-rate));
            // camRight.z = (camRight.z * cos(-rate)) + (camLook.z * sin(-rate));

            camLook = (tempCamLook * cos(-rate)) - (tempCamRight * sin(-rate));
			// camLook.x = (camLook.x * cos(-rate)) - (camRight.x * sin(-rate));
            // camLook.y = (camLook.y * cos(-rate)) - (camRight.y * sin(-rate));
            // camLook.z = (camLook.z * cos(-rate)) - (camRight.z * sin(-rate));
			break;

        case '3':
            camLook = (tempCamLook * cos(rate)) + (tempCamUp * sin(rate));
			// camLook.x = (camLook.x * cos(rate)) + (camUp.x * sin(rate));
            // camLook.y = (camLook.y * cos(rate)) + (camUp.y * sin(rate));
            // camLook.z = (camLook.z * cos(rate)) + (camUp.z * sin(rate));

            camUp = (tempCamUp * cos(rate)) - (tempCamLook * sin(rate));
			// camUp.x = (camUp.x * cos(rate)) - (camLook.x * sin(rate));
            // camUp.y = (camUp.y * cos(rate)) - (camLook.y * sin(rate));
            // camUp.z = (camUp.z * cos(rate)) - (camLook.z * sin(rate));
			break;

        case '4':
            camLook = (tempCamLook * cos(-rate)) + (tempCamUp * sin(-rate));
            // camLook.x = (camLook.x * cos(-rate)) + (camUp.x * sin(-rate));
            // camLook.y = (camLook.y * cos(-rate)) + (camUp.y * sin(-rate));
            // camLook.z = (camLook.z * cos(-rate)) + (camUp.z * sin(-rate));

            camUp = (tempCamUp * cos(-rate)) - (tempCamLook * sin(-rate));
            // camUp.x = (camUp.x * cos(-rate)) - (camLook.x * sin(-rate));
            // camUp.y = (camUp.y * cos(-rate)) - (camLook.y * sin(-rate));
            // camUp.z = (camUp.z * cos(-rate)) - (camLook.z * sin(-rate));
			break;

        case '5':
            camUp = (tempCamUp * cos(rate)) + (tempCamRight * sin(rate));
            // camUp.x = (camUp.x * cos(rate)) + (camRight.x * sin(rate));
            // camUp.y = (camUp.y * cos(rate)) + (camRight.y * sin(rate));
            // camUp.z = (camUp.z * cos(rate)) + (camRight.z * sin(rate));
			
            camRight = (tempCamRight * cos(rate)) - (tempCamUp * sin(rate));
            // camRight.x = (camRight.x * cos(rate)) - (camUp.x * sin(rate));
            // camRight.y = (camRight.y * cos(rate)) - (camUp.y * sin(rate));
            // camRight.z = (camRight.z * cos(rate)) - (camUp.z * sin(rate));
			break;

        case '6':
            camUp = (tempCamUp * cos(-rate)) + (tempCamRight * sin(-rate));
            // camUp.x = (camUp.x * cos(-rate)) + (camRight.x * sin(-rate));
            // camUp.y = (camUp.y * cos(-rate)) + (camRight.y * sin(-rate));
            // camUp.z = (camUp.z * cos(-rate)) + (camRight.z * sin(-rate));

            camRight = (tempCamRight * cos(-rate)) - (tempCamUp * sin(-rate));
            // camRight.x = (camRight.x * cos(-rate)) - (camUp.x * sin(-rate));
            // camRight.y = (camRight.y * cos(-rate)) - (camUp.y * sin(-rate));
            // camRight.z = (camRight.z * cos(-rate)) - (camUp.z * sin(-rate));
			break;
        case ' ':
            textureMode = !textureMode;
            objects[0]->setTextureMode(textureMode);
        case 27:    // ESC key
            exit(0);    // Exit window
            break;
    }
    camLook.normalize();
    camRight.normalize();
    camUp.normalize();
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y)
{
    double rate = 10.0;
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			camPos = camPos + camLook * rate;
			break;

		case GLUT_KEY_DOWN:		// up arrow key
            camPos = camPos - camLook * rate;
			break;

		case GLUT_KEY_RIGHT:
            camPos = camPos + camRight * rate;
			break;

		case GLUT_KEY_LEFT :
            camPos = camPos - camRight * rate;
			break;

		case GLUT_KEY_PAGE_UP:
            camPos = camPos + camUp *  rate;
			break;

		case GLUT_KEY_PAGE_DOWN:
            camPos = camPos - camUp * rate;
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    loadData();
    //checkData();
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(windowWidth, windowHeight);               // Set the window's initial width & height
    glutInitWindowPosition(0, 0);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Ray tracing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL(); // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}