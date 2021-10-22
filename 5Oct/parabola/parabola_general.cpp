// A simple OpenGL program that draws a parabola.

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
void draw_parabola(float cx, float cy,  float a);
void plot_quad(float x1, float y1, float cx, float cy);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Parabola - using general formula");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    float a = rand() % 100;
    float cx = rand() % 1000 - 500;
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
        draw_parabola(cx, cy, a);
    glFlush();
}

/************************ Drawing utility functions *************************/
void draw_parabola(float cx, float cy, float a) {
    glPointSize(2);
    glBegin(GL_POINTS);
    float x_i = 0;
    float y_i;
    while(x_i <= 1000) {
        y_i = 2 * pow(a*x_i, 0.5);
        plot_quad(x_i, y_i, cx, cy);
        x_i++;
    }
    glEnd();
}

void plot_quad(float x1, float y1, float cx, float cy) {
    glPointSize(2);
    glBegin(GL_POINTS);
        glVertex2i(x1+cx, y1+cy);
        glVertex2i(x1+cx, -y1+cy);
        glVertex2i(y1+cx, x1+cy);
        glVertex2i(-y1+cx, x1+cy);
    glEnd();
}