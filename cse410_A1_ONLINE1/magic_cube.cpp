#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include <cmath>

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

struct cameraPoint {
    GLfloat x, y, z;
    cameraPoint( GLfloat x, GLfloat y, GLfloat z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct cameraPoint pos = cameraPoint(4,4,4);   // position of the eye
struct cameraPoint l = cameraPoint(-4,-4,-4);     // look/forward direction
struct cameraPoint r = cameraPoint(1,0,0);     // right direction
struct cameraPoint u = cameraPoint(0,1,0);     // up direction

class Point {
public:
    float x;
    float y;
    float z;
};

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

GLdouble vertices[3][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
GLdouble delX = 0.0, delY = 0.0, delZ = 0.0;
GLdouble scale = 0.0;
GLdouble radius = 1.0/sqrt(3);
GLdouble theta_max = (M_PI-acos(-1.0/3.0));

GLdouble rotateAngle = 0.0;

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,2,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,2);
    glEnd();
}

void drawTriangle() {
    glBegin(GL_TRIANGLES);
        glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
        glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
        glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);  
     glEnd();
}

Point** buildUnitPositiveX(int subdivision)
{
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    Point **points = new Point *[pointsPerRow];
    for (int i = 0; i < pointsPerRow; ++i)
        points[i] = new Point[pointsPerRow];

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i) {
    // normal for latitudinal plane
    // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
    // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = M_PI / 4.0 - (M_PI / 2.0 * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-r.)
        for (unsigned int j = 0; j < pointsPerRow; ++j) {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = -M_PI / 4.0 + (M_PI / 2.0 * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float norm_scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= norm_scale;
            v[1] *= norm_scale;
            v[2] *= norm_scale;

            // add a vertex into array
            points[i][j].x = v[0] * 1/sqrt(3);
            points[i][j].y = v[1] * 1/sqrt(3);
            points[i][j].z = v[2] * 1/sqrt(3);
        }
    }
  return points;
}

void drawSphere(int subdivision) {
    Point** spherePoints = buildUnitPositiveX(3);
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    glBegin(GL_QUADS);
    for(int i = 0; i < pointsPerRow - 1; i++) {
        for(int j = 0; j < pointsPerRow - 1; j++) {
            glVertex3f(spherePoints[i][j].x, spherePoints[i][j].y, spherePoints[i][j].z);
            glVertex3f(spherePoints[i][j+1].x, spherePoints[i][j+1].y, spherePoints[i][j+1].z);
            glVertex3f(spherePoints[i+1][j+1].x, spherePoints[i+1][j+1].y, spherePoints[i+1][j+1].z);
            glVertex3f(spherePoints[i+1][j].x, spherePoints[i+1][j].y, spherePoints[i+1][j].z);
        }
    }
    glEnd();
}

void drawOctahedron() {

    for(int i  = 0; i < 4; i++) {
        if(i & 1)
            glColor3f(1.0f, 0.0f, 1.0f);
        else
            glColor3f(0.0f, 1.0f, 1.0f);
        glPushMatrix();
            glRotatef(i*90.0, 0, 1, 0);
            glTranslatef(scale/3.0, scale/3.0, scale/3.0);
            glScalef(1-scale,1-scale,1-scale);
            drawTriangle(); 
        glPopMatrix();

        if(i & 1)
            glColor3f(0.0f, 1.0f, 1.0f);
        else
            glColor3f(1.0f, 0.0f, 1.0f);
        glPushMatrix();
            glRotatef(i*90.0, 0, 1, 0);
            glRotatef(180.0, 1, 0, 1);
            glTranslatef(scale/3.0, scale/3.0, scale/3.0);
            glScalef(1-scale,1-scale,1-scale);
            drawTriangle();
        glPopMatrix();
    }    

}

// void drawPyramid() {
//     glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
//         // Front
//         glColor3f(1.0f, 0.0f, 1.0f);     
//         glPushMatrix();
//             glTranslatef(scale/3.0, scale/3.0, scale/3.0);
//             glScalef(1-scale,1-scale,1-scale);
//             drawTriangle(); 
//         glPopMatrix();
//         glRotatef(90.0, 0, 1, 0);


//         // Right
//         glColor3f(0.0f, 1.0f, 1.0f);     
//         glPushMatrix();
//             glTranslatef(scale/3.0, scale/3.0, scale/3.0);
//             glScalef(1-scale,1-scale,1-scale);
//             drawTriangle(); 
//         glPopMatrix();
//          glRotatef(90.0, 0, 1, 0);

//         // Back
//         glColor3f(0.0f, 1.0f, 0.0f);     // Green
//         glVertex3f(0.0f, 1.0f, 0.0f);
//         glVertex3f(1.0f, -1.0f, -1.0f);
//         glVertex3f(-1.0f, -1.0f, -1.0f);

//         // Left
//         glColor3f(0.0f,1.0f,1.0f);       // Cyan
//         glVertex3f( 0.0f, 1.0f, 0.0f);
//         glVertex3f(-1.0f,-1.0f,-1.0f);
//         glVertex3f(-1.0f,-1.0f, 1.0f);
//     glEnd();   // Done drawing the pyramid
// }



void drawAllSpheres() {

    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(1.0-scale, 0.0, 0.0);
    glScalef(scale,scale,scale);
    drawSphere(3);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0, 0, 1, 0);
    glTranslatef(1.0-scale, 0.0, 0.0);
    glScalef(scale,scale,scale);
    drawSphere(3);
    glPopMatrix();
   
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0);
    glTranslatef(1.0-scale, 0.0, 0.0);
    glScalef(scale,scale,scale);
    drawSphere(3);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270.0, 0, 1, 0);
    glTranslatef(1.0-scale, 0.0, 0.0);
    glScalef(scale,scale,scale);
    drawSphere(3);
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(270.0, 0, 0, 1);
    glTranslatef(1.0-scale, 0.0, 0.0);
    glScalef(scale,scale,scale);
    drawSphere(3);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0, 0, 0, 1);
    glTranslatef(1.0-scale, 0.0, 0.0);
    glScalef(scale,scale,scale);
    drawSphere(3);
    glPopMatrix();
}

void drawCylinder(double height = sqrt(2), double radius = 1.0/sqrt(3), int segments = 36) {
    
    //float t = M_PI-acos(-1.0/3.0);
    height *= (1-scale);
    radius *= scale;
    glTranslatef((1-scale)/sqrt(2),0,0);
    glRotatef(-70.5287794/2,0,0,1);
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * theta_max / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}
/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */

void drawAllCylinders() {

    
    glColor3f(1.0,1.0,0.0);

    glPushMatrix();
    glRotatef(45.0,0,1,0); //rotate along y axis
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        drawCylinder();
        glPopMatrix();
        glRotatef(90.0,0,1,0); //rotate along y axis
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0,1,0,0);  //rotate along x axis
    glRotatef(45.0,0,1,0); //rotate along y axis
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        drawCylinder();
        glPopMatrix();
        glRotatef(90.0,0,1,0); //rotate along y axis
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0,0,0,1);  //rotate along z axis
    glRotatef(45.0,0,1,0); //rotate along y axis
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        drawCylinder();
        glPopMatrix();
        glRotatef(90.0,0,1,0); //rotate along y axis
    }
    glPopMatrix();
}
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);
    r.x = (l.y*u.z - l.z*u.y);
    r.y = -(l.x*u.z - l.z*u.x);
    r.z = (l.x*u.y - l.y*u.x);
    // control viewing (or camera)
    gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);
    
    //drawAxes();
    glPushMatrix();
    glRotatef(rotateAngle,0,1,0);
    drawOctahedron();
    drawAllSpheres();
    drawAllCylinders();
    glPopMatrix();

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
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    double rate = 0.01;
    switch (key) {
    // Control eye (location of the eye)
    // control eyex
        case ',':
            //make the triangles smaller, spheres larger
            scale += 1.0/16.0;
            if(scale >= 1.0) 
                scale = 1.0;
            break;
        case '.':
            //make the triangle larger, spheres smaller
            scale -= 1.0/16.0;
            if(scale <= 0.0) 
                scale = 0.0;
            break;
        case 'a':
            rotateAngle -= 5.0;
            if(rotateAngle <= -360.0) 
                rotateAngle += 360.0;
            break;
        case 'd':
            rotateAngle += 5.0;
            if(rotateAngle >= 360.0) 
                rotateAngle -= 360.0;
            break;
        case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;
        case 27:    // ESC key
            exit(0);    // Exit window
            break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y)
{
    double rate = 0.01;
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x*rate;
			pos.y+=l.y*rate;
			pos.z+=l.z*rate;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x*rate;
			pos.y-=l.y*rate;
			pos.z-=l.z*rate;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x*rate;
			pos.y+=r.y*rate;
			pos.z+=r.z*rate;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x*rate;
			pos.y-=r.y*rate;
			pos.z-=r.z*rate;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Magic Cube");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    
    return 0;
}