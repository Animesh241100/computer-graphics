// A simple OpenGL program that draws a car.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

#define PI 3.14159     // Mathematical Constant PI


using namespace std;

void draw_image();
void draw_life();

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game Of Life!");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.8, 1.0, 1.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_life();
}

/************************ Drawing utility functions *************************/
void draw_life() {
    glPointSize(15);
    int i = 0;
    while(1) {
        glBegin(GL_POINTS);
        glVertex2d(400-i, 400-i);
        glVertex2d(400-i, -400+i);
        glVertex2d(-400+i, 400-i);
        glVertex2d(-400+i, -400+i);
        usleep(6000);
        glEnd();
        glFlush();
        glClear(GL_COLOR_BUFFER_BIT);
        i += 5;
    }
}
