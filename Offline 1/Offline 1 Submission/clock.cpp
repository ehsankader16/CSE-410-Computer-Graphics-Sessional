#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <ctime>
#include <GL/glut.h>

GLdouble hr = 0.0;
GLdouble mnt = 0.0;
GLdouble sec = 0.0;
GLdouble t = 0.0;

GLdouble secHandLen = 0.3;
GLdouble mntHandLen = 0.24;
GLdouble hrHandLen = 0.16;
GLdouble penLen = 0.4;

GLdouble secAngle = 90.0;
GLdouble mntAngle = 90.0;
GLdouble hrAngle = 90.0;
GLdouble penAngle = 0.0;

GLint dir = 0;

void drawSecondHand() {
    glLineWidth(10);
    glPushMatrix();
    glRotated(secAngle, 0, 0, 1);
    glBegin(GL_POLYGON);
        glVertex2d(0.0,0.0);
        glVertex2d(-0.01,secHandLen*2.7/3.0);
        glVertex2d(0.0,secHandLen);
        glVertex2d(0.01,secHandLen*2.7/3.0);
    glEnd();
    glPopMatrix();
}

void drawMinuteHand() {
    glLineWidth(10);
    glPushMatrix();
    glRotated(mntAngle, 0, 0, 1);
    glBegin(GL_POLYGON);
        glVertex2d(0.0,0.0);
        glVertex2d(0.015,mntHandLen*2.3/3.0);
        glVertex2d(0.0,mntHandLen);
        glVertex2d(-0.015,mntHandLen*2.3/3.0);
    glEnd();
    glPopMatrix();
}

void drawHourHand() {
    glLineWidth(20);
    glPushMatrix();
    glRotated(hrAngle, 0, 0, 1);
    glBegin(GL_POLYGON);
        glVertex2d(0.0,0.0);
        glVertex2d(-0.02,hrHandLen*2.0/3.0);
        glVertex2d(0.0,hrHandLen);
        glVertex2d(0.02,hrHandLen*2.0/3.0);
    glEnd();
    glPopMatrix();
}


void drawCircle(double cx, double cy, double r,bool polygon = false) {
    glColor3d(1,1,1);
    if(polygon)
        glBegin(GL_POLYGON);
    else
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)   {
            //float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
            double theta = double(i) * M_PI /180.0;//get the current angle
            double x = r * cos(theta);//calculate the x component 
            double y = r * sin(theta);//calculate the y component 
            glVertex2d(x + cx, y + cy);//output vertex 
        }
    glEnd();
}

void drawTicks(double cx, double cy, double r) {
    for(int i  = 0; i < 12; i++) {
        glPushMatrix();
            glRotated(i*30, 0, 0, 1);
            glBegin(GL_LINES);
                if(i%3 == 0) {
                    glLineWidth(20);
                    glVertex2d(cx+r-0.08,cy);
                } else {
                    glLineWidth(5);
                    glVertex2d(cx+r-0.03,cy);
                }
                glVertex2d(cx+r,cy);
            glEnd(); 
        glPopMatrix();
    }  
}

void drawPendulum(double tx, double ty) {
    //glLineWidth(20);
    glPushMatrix();
    glTranslatef(tx,ty,0.0);
    glRotated(penAngle, 0, 0, 1);
    drawCircle(0.0, 0.0, 0.03, true);
    glBegin(GL_LINES);
        glVertex2d(0.0,0.0);
        glVertex2d(0.0,-penLen);
    glEnd();
    drawCircle(0.0,-penLen, 0.05, true);
    glPopMatrix();
}

void drawOuterBody() {
    glLineWidth(5);
    glBegin(GL_LINES);
        glVertex2d(-0.6,0.9);
        glVertex2d(0.6,0.9);
    glEnd();

    glBegin(GL_LINES);
        glVertex2d(-0.6,0.9);
        glVertex2d(-0.6, -0.5);
    glEnd();

    glBegin(GL_LINES);
        glVertex2d(0.6,0.9);
        glVertex2d(0.6,-0.5);
    glEnd();

    glBegin(GL_LINES);
        glVertex2d(0.6,-0.5);
        glVertex2d( 0.0, -0.8);
    glEnd();

    glBegin(GL_LINES);
        glVertex2d(-0.6,-0.5);
        glVertex2d(0.0,-0.8);
    glEnd();
}
void display() { 
    glClear(GL_COLOR_BUFFER_BIT);

    drawOuterBody();
    glPushMatrix();
        glTranslatef(0.0,0.2,0.0);
        drawSecondHand();
        drawMinuteHand();
        drawHourHand();
        drawCircle(0.0, 0.0, 0.40);
        drawTicks(0.0, 0.0, 0.40);
        drawCircle(0.0, 0.0, 0.50);
        drawPendulum(0.0,-0.45);
    glPopMatrix();

    glFlush(); 
}


void timerListener(int value) {
    secAngle -= 6;
    mntAngle -= 1.0/10;   //(1/60)*6
    hrAngle -=  1.0/120;    //(1/3600)*6*5
    glutPostRedisplay();
    glutTimerFunc(1000, timerListener, 0);
}

void penTimerListener(int value) {
    t += 0.1;
    penAngle = 45.0 * cos(M_PI*t);
    
    glutPostRedisplay();
    glutTimerFunc(100, penTimerListener, 0);
}
/*Main function.: GLUT runs as a console starting at main()*/
int main(int argc, char **argv){
    time_t ttime = time(0);
    tm *local_time = localtime(&ttime);
    hr = (local_time->tm_hour)%12;
    mnt = local_time->tm_min;
    sec = local_time->tm_sec;
    //std::cout<<hr<<" "<<mnt<<"\n";
    secAngle = - sec*6.0;
    mntAngle = - mnt*6.0 - sec/12.0;
    hrAngle = - hr*30.0 - mnt/2.0;
    //std::cout<<hrAngle<<" "<<mntAngle<<"\n";

	glutInit(&argc,argv);               //initialize GLUT
	glutInitWindowSize(800, 800);       //Set the window's initial size & width
	glutInitWindowPosition(100, 50);    //Position the window's initial top left corner
	glutCreateWindow("Clock"); //Create a window with given title
	glutDisplayFunc(display);	        //Register display callback handler for window re-paint
    //glutIdleFunc(idle);                 //Register callback handler if no other event
    glutTimerFunc(1000, timerListener, 0);
    glutTimerFunc(100, penTimerListener, 0);
	glutMainLoop();		                //Enter the infinite event-processing loop
	return 0;
} 