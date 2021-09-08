// A simple OpenGL program that draws a polygon.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#define PI 3.14159     // Mathematical Constant PI

void draw_square();
void draw_triangle();
void draw_strinp();
void draw_house();
void draw_right_window();
void draw_left_window();
void draw_door();
void draw_mountain1();
void draw_mountain2();

float convert_degree_to_rad(int angle_deg) {
    return (float)angle_deg * (PI) / (float)180;
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

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a House");
    glutDisplayFunc(draw_house);
    glutMainLoop();
    return 0;
}


void draw_house() {
    glClearColor(1.0, 1.0, 0.4, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        draw_mountain2();
        draw_mountain1();
        draw_square();
        draw_strinp();
        draw_triangle();
        draw_right_window();
        draw_left_window();
        draw_door();
    glFlush();
}

void draw_square() {
    glColor3f(0.0, 0.6, 0.6);
        glBegin(GL_POLYGON);
                glVertex2f(0.4, 0);
                glVertex2f(0.4, -0.8);
                glVertex2f(-0.4, -0.8);
                glVertex2f(-0.4, 0);
        glEnd();
    glFlush();
}

void draw_triangle() {
    glColor3f(0.8, 0.2, 0.0);
        glBegin(GL_POLYGON);
            glVertex2f(0.4, 0.01);
            glVertex2f(-0.4, 0.01);
            glVertex2f(0, 0.7);
        glEnd();
    glFlush();
}

void draw_strinp() {
    glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_POLYGON);
            glVertex2f(0.4, 0);
            glVertex2f(0.4, 0.01);
            glVertex2f(-0.4, 0.01);
            glVertex2f(-0.4, 0);
        glEnd();
    glFlush();
}


void draw_right_window() {
    glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
            glVertex2f(0.20, -0.35);
            glVertex2f(0.30, -0.35);
            glVertex2f(0.30, -0.45);
            glVertex2f(0.20, -0.45);
        glEnd();
    glFlush();
}

void draw_left_window() {
    // glColor3f(1.0, 1.0, 1.0);
        // glBegin(GL_POLYGON);
            draw_wheel(-0.25, -0.3, 1, 0.1, 1.0, 1.0, 1.0);
            // glVertex2f(-0.20, -0.35);
            // glVertex2f(-0.30, -0.35);
            // glVertex2f(-0.30, -0.45);
            // glVertex2f(-0.20, -0.45);
        // glEnd();
    // glFlush();
}

void draw_door() {
    glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
            glVertex2f(0.10, -0.35);
            glVertex2f(0.10, -0.8);
            glVertex2f(-0.10, -0.8);
            glVertex2f(-0.10, -0.35);
        glEnd();
    glFlush();
}


void draw_mountain1() {
    glColor3f(0.2, 0.2, 0.85);
        glBegin(GL_POLYGON);
            glVertex2f(-1.0, -1.0);
            glVertex2f(1.0, 1.0);
            glVertex2f(1.0, -1.0);
        glEnd();
    glFlush();
}

void draw_mountain2() {
    glColor3f(0.0, 0.1, 0.8);
        glBegin(GL_POLYGON);
            glVertex2f(0.8, 0.3);
            glVertex2f(-1.0, 1.0);
            glVertex2f(-1.0, -1.0);
        glEnd();
    glFlush();
}



