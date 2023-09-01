#include <iostream>
#include <math.h>
#include <GL/glut.h>

GLdouble px = 0, py = 0;
GLdouble vx = 0.01;

void display() { 
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(30);
    glBegin(GL_POINTS);
        glColor3d(1,0,0);
        glVertex2d(px,py);
    glEnd();

    glFlush(); 
}

void idle() {
    px += vx; 
    if (px  >= 1 || px <= -1)
        vx = -vx;
    glutPostRedisplay();
}

void timerListener(int value) {
    px += vx; 
    if (px  >= 1 || px <= -1)
        vx = -vx;
    glutPostRedisplay();
    glutTimerFunc(100, timerListener, 0);
}

/*Main function.: GLUT runs as a console starting at main()*/
int main(int argc, char **argv){
	glutInit(&argc,argv);               //initialize GLUT
	glutInitWindowSize(640, 640);       //Set the window's initial size & width
	glutInitWindowPosition(100, 50);    //Position the window's initial top left corner
	glutCreateWindow("OpenGL lecture"); //Create a window with given title
	glutDisplayFunc(display);	        //Register display callback handler for window re-paint
    //glutIdleFunc(idle);                 //Register callback handler if no other event
    glutTimerFunc(10, timerListener, 0);
	glutMainLoop();		                //Enter the infinite event-processing loop
	return 0;
} 