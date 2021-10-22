// A simple OpenGL program that draws a hyperbola.

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
void draw_hyperbola(float cx, float cy,  float rx, float ry);

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hyperbola - using parametric coordinates");
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
    // rx = 500;
    // ry = 100;
    // cx = 0;
    // cy = 0;
    float r = 0.1 * (rand() % 10);
    float g = 0.1 * (rand() % 10);
    float b = 0.1 * (rand() % 10);
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(r, g, b);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_hyperbola(cx, cy, rx, ry);
    glFlush();
}

/************************ Drawing utility functions *************************/
void draw_hyperbola(float cx, float cy, float rx, float ry) {
    float p = 4*rx - 4;
    float theta = 0;
    int cnt = p;  
    glPointSize(3);
    glBegin(GL_POINTS);
    while(cnt--) {
        glVertex2i(round(cx + rx/cos(theta)), round(cy + ry*tan(theta))); // x = a*sec(theta); y = b*tan(theta)
        theta += (float)2*PI / (float)p;
    }
    glEnd();
}
