#include <iostream>
#include <math.h>
#include <GL/glut.h>

GLdouble px = 0, py = 0;
GLdouble vx = 0.1;

void display() { 
    glClear(GL_COLOR_BUFFER_BIT);

    // glLineWidth(10);
    // glBegin(GL_TRIANGLES);
    //     glColor3d(1,0,0);
    //     glVertex2d(px,py);
    //     glColor3d(0,1,0);
    //     glVertex2d(px+0.5,py);
    //     glColor3d(0,0,1);
    //     glVertex2d(px,py+0.5);
    // glEnd();

    // glLineWidth(10);
    // glBegin(GL_LINES);
    //     glColor3d(1,0,0);
    //     glVertex2d(px,py);
    //     glColor3d(0,1,0);
    //     glVertex2d(px+0.5,py);
    // glEnd();

    // px += 0.1;
    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3d(1,0,0);
        glVertex2d(px,py);
    glEnd();

    // glPointSize(10); 
    // glBegin(GL_POINTS);
    //     glVertex2d(0,0);
    //     glVertex2d(0.5,0);
    //     glVertex2d(0,0.5);
    // glEnd();

    // glBegin(GL_LINES);
    //     glVertex2d(0,0);
    //     glVertex2d(0.5,0);

    //     glVertex2d(0,0.5);
    //     glVertex2d(2,2);
    // glEnd();

    // glBegin(GL_LINE_STRIP);
    //     glVertex2d(0,0);
    //     glVertex2d(0.5,0);
    //     glVertex2d(0,0.5);
    // glEnd();

    // glLineWidth(5);
    // glBegin(GL_LINE_LOOP);
    //     glVertex2d(0,0);
    //     glVertex2d(0.5,0);
    //     glVertex2d(0,0.5);
    // glEnd();

    // glBegin(GL_TRIANGLES);
    //     glVertex2d(0,0);
    //     glVertex2d(0.5,0);
    //     glVertex2d(0,0.5);
    // glEnd();

    // glBegin(GL_QUADS);
    //     glVertex2d(0,0);
    //     glVertex2d(0.5,0);
    //     glVertex2d(0.5,0.5);
    //     glVertex2d(0,0.5);
    // glEnd();

    glFlush(); 
}

void idle() {
    px += vx; 
    if (px  >= 1 || px <= -1)
        vx = -vx;
    glutPostRedisplay();
    // glClear(GL_COLOR_BUFFER_BIT);
    // glPointSize(10);
    // glBegin(GL_POINTS);
    //     glColor3d(1,0,0);
    //     glVertex2d(px,py);
    // glEnd();

    //glFlush();

}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 'a':
            px -= 0.1;
            break;
        case 'd':
            px += 0.1;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            px -= 0.1;
            break;
        case GLUT_KEY_RIGHT:
            px += 0.1;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        px = -1 + x * 2.0 / 640.0;
        py = 1 - y * 2.0 / 640.0;
        glutPostRedisplay();
    }
}
int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 50);

	glutCreateWindow("My OpenGL Program");
	glutDisplayFunc(display);	//display callback function
    //glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    // glClearColor(1,1,1,0);
	glutMainLoop();		//The main loop of OpenGL

	return 0;
} 