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
void draw_ellipse(float cx, float cy,  float rx, float ry);

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ellipse - using general formula");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    float rx = rand() % 500;
    float cx = rand() % 1000 - 500;
    float ry = rand() % 500;
    float cy = rand() % 1000 - 500;
    float r = 0.1 * (rand() % 10);
    float g = 0.1 * (rand() % 10);
    float b = 0.1 * (rand() % 10);
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(r, g, b);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_ellipse(cx, cy, rx, ry);
    glFlush();
}

/************************ Drawing utility functions *************************/
void draw_ellipse(float cx, float cy, float rx, float ry) {
    glPointSize(3);
    glBegin(GL_POINTS);
    float x_i = -rx;
    float y_i;
    while(x_i <= rx) {
        y_i = ((float)ry/(float)rx) * pow(rx*rx - x_i*x_i, 0.5);
        glVertex2i(cx+round(x_i), cy+round(y_i));
        glVertex2i(cx+round(x_i), cy+(-1)*round(y_i));
        x_i++;
    }
    glEnd();
}
