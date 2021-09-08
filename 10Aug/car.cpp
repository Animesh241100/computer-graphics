// A simple OpenGL program that draws a polygon.
#include<math.h>
#include "GL/freeglut.h"
#include "GL/gl.h"

#define PI 3.14159     // Mathematical Constant PI

void draw_car();
void draw_upper_box();
void draw_lower_box();
void draw_wheel(float x, float y, int step_angle_deg, float radius, float r, float g, float b);
float convert_degree_to_rad(int angle_deg);
void draw_window_right();
void draw_window_left();
void draw_front_box();

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a Car");
    glutDisplayFunc(draw_car);
    glutMainLoop();
    return 0;
}


void draw_car() {
    glClearColor(0.1, 0.0, 0.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        draw_front_box();
        draw_upper_box();
        draw_lower_box();
        draw_wheel(0.57, -0.03, 5, 0.13, 0.1, 0.1, 0.2);
        draw_wheel(-0.52, -0.03, 5, 0.13, 0.1, 0.1, 0.2);
        draw_window_left();
        draw_window_right();
    glFlush();
}

void draw_window_left() {
    glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_POLYGON);
            glVertex2f(-0.4, 0.17);
            glVertex2f(-0.02, 0.37);
            glVertex2f(0.42, 0.36);
            glVertex2f(0.4, 0.17);
        glEnd();
    glFlush();
}

void draw_window_right() {
    glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_POLYGON);
            glVertex2f(0.43, 0.17);
            glVertex2f(0.45, 0.35);
            glVertex2f(0.7, 0.26);
            glVertex2f(0.73, 0.17);
        glEnd();
    glFlush();
}

void draw_wheel(float x, float y, int step_angle_deg, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
        glBegin(GL_POLYGON);
            float step_angle_rad = convert_degree_to_rad(step_angle_deg);    
            float present_angle_rad = step_angle_rad;
            float x1, y1;
            while(present_angle_rad <= 2*PI) {
                x1 = x + radius * cos(present_angle_rad);
                y1 = y + radius * sin(present_angle_rad);
                glVertex2f(x1, y1);
                present_angle_rad += step_angle_rad;
            }
        glEnd();
    glFlush();
}

float convert_degree_to_rad(int angle_deg) {
    return (float)angle_deg * (PI) / (float)180;
}


void draw_front_box() {
    glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POLYGON);
            // glVertex2f(-0.8, 0.14);
            glVertex2f(-0.8, 0.12);
            glVertex2f(-0.8, 0.17);
            glVertex2f(-0.45, 0.18);
            glVertex2f(-0.45, 0.12);
        glEnd();
    glFlush();
}


void draw_upper_box() {
    glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POLYGON);
            // glVertex2f(-0.8, 0.14);
            glVertex2f(-0.02, 0.4);
            glVertex2f(0.5, 0.38);
            glVertex2f(0.7, 0.3);
            glVertex2f(0.8, 0.12);
            glVertex2f(-0.55, 0.12);
        glEnd();
    glFlush();
}


void draw_lower_box() {
    glColor3f(0.8, 0.8, 0.0);
        glBegin(GL_POLYGON);
            glVertex2f(0.8, 0.12);
            glVertex2f(0.8, -0.04);
            glVertex2f(-0.8, -0.04);
            glVertex2f(-0.8, 0.12);
        glEnd();
    glFlush();
}
