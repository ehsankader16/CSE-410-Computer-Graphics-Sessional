#include <iostream>
#include <math.h>
#include <GL/glut.h>

GLdouble sa = 0, dsa = 1;

GLdouble ta = 0, dta = 0.5;

void drawTriangle() {
    glColor3d(0,1,0);
    glBegin(GL_TRIANGLES);
        glVertex2d(-0.5,0.5);
        glVertex2d(0.0,1.0);
        glVertex2d(0.5,0.5);
    glEnd();

}

void drawSquare() {
    glColor3d(1,0,0);
    glBegin(GL_QUADS);
        glVertex2d(-0.5,0.5);
        glVertex2d(-0.5,-0.5);
        glVertex2d(0.5,-0.5);
        glVertex2d(0.5,0.5);
    glEnd();
}

void display() { 
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    
    glScaled(0.1,0.1,0.1);

    glPushMatrix();
        glRotated(sa, 0, 0, 1);
        drawSquare();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0,5,0);
        glRotated(ta, 0, 0, 1);
        drawTriangle();
    glPopMatrix();


    // drawTriangle();
    // glRotated(90,0,0,1);
    // drawTriangle();
    // glRotated(90,0,0,1);
    // drawTriangle();
    // glRotated(90,0,0,1);
    // drawTriangle();

    // glTranslated(0.5,0.5,0);
    // glScaled(2,2,1);

    // glBegin(GL_QUADS);
    //     glVertex2d(0,0);
    //     glVertex2d(0,0.1);
    //     glVertex2d(0.1,0.1);
    //     glVertex2d(0.1,0);
    // glEnd();

    glFlush(); 
}

void idle() {
    sa += dsa;
    ta += dta;
    glutPostRedisplay();
}

/*Main function.: GLUT runs as a console starting at main()*/
int main(int argc, char **argv){
	glutInit(&argc,argv);               //initialize GLUT
	glutInitWindowSize(640, 640);       //Set the window's initial size & width
	glutInitWindowPosition(100, 50);    //Position the window's initial top left corner
	glutCreateWindow("OpenGL lecture"); //Create a window with given title
	glutDisplayFunc(display);	        //Register display callback handler for window re-paint
    glutIdleFunc(idle);
	glutMainLoop();		                //Enter the infinite event-processing loop
	return 0;
} 