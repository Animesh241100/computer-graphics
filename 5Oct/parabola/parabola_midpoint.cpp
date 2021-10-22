// A simple OpenGL program that draws a Parabola.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

using namespace std;

void draw_image();
void draw_parabola(float cx, float cy,  float a);
void draw_region2(float cx, float cy,  float a, float xi, float yi);
void plot_quad(float x1, float y1, float cx, float cy);

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Parabola - using midpoint algorithm");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    float a = rand() % 500;
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
    float x_i = 0;
    float y_i = 0;
    float p_i = 1 - 2*a;
    while(y_i < 2*a) {
        y_i++;
        if(p_i < 0)
            p_i += 1 + 2*y_i;    
        else {
            x_i++;
            p_i += 1 + 2*y_i - 4*a;
        }
        plot_quad(x_i, y_i, cx, cy);
    }
    draw_region2(cx, cy, a, x_i, y_i);
}

void draw_region2(float cx, float cy, float a, float xi, float yi) {
    float x_i = xi;
    float y_i = yi;
    float p_i = pow(y_i+0.5, 2) - 4*a*(x_i+1);
    while(x_i <= 1000) {
        x_i++;
        if(p_i > 0)
            p_i += -4*a;    
        else {
            y_i++;
            p_i += -4*a + 2*y_i;    
        }
        plot_quad(x_i, y_i, cx, cy);
    }
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

