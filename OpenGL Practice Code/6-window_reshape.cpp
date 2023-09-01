#include <iostream>
#include <math.h>
#include <GL/glut.h>


void display() { 
    glClear(GL_COLOR_BUFFER_BIT);


    glBegin(GL_QUADS);
        glColor3d(1,0,0);
        glVertex2d(0,0.5);
        glVertex2d(0.5,0.5);
        glVertex2d(0.5,0);
        glVertex2d(0,0);
    glEnd();

    glFlush(); 
}

void reshape(int width, int height) {
    //std::cout << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    
    if (width >= height)
        gluOrtho2D(-width * 1.0/height,width * 1.0/height,-1,1);
    else
        gluOrtho2D(-1, 1,-height * 1.0/width, height * 1.0/width);
}

/*Main function.: GLUT runs as a console starting at main()*/
int main(int argc, char **argv){
	glutInit(&argc,argv);               //initialize GLUT
	glutInitWindowSize(640, 640);       //Set the window's initial size & width
	glutInitWindowPosition(100, 50);    //Position the window's initial top left corner
	glutCreateWindow("OpenGL lecture"); //Create a window with given title
	glutDisplayFunc(display);	        //Register display callback handler for window re-paint
    glutReshapeFunc(reshape);
	glutMainLoop();		                //Enter the infinite event-processing loop
	return 0;
} 