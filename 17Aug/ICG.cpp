// A simple OpenGL program that draws a polygon.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#define PI 3.14159     // Mathematical Constant PI

void draw_ICG();
void draw_I();
void draw_C();
void draw_G();

void draw_circle(float x, float y, int step_angle_deg, float radius, float r, float g, float b, float start_comp, float end_comp);
float get_random_color();
float convert_degree_to_rad(int angle_deg);

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a House");
    glutDisplayFunc(draw_ICG);
    glutMainLoop();
    return 0;
}


void draw_ICG() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        draw_I();
        draw_C();
        draw_G();
    glFlush();
}


void draw_I() {
    float r = get_random_color();
    float g = get_random_color();
    float b = get_random_color();
    glColor3f(r, g, b);
        glBegin(GL_POLYGON);
                glVertex2f(-0.4, 0.3);
                glVertex2f(-0.5, 0.3);
                glVertex2f(-0.5, -0.3);
                glVertex2f(-0.4, -0.3);
        glEnd();
    glFlush();
}

void draw_C() {
    float r = get_random_color();
    float g = get_random_color();
    float b = get_random_color();
    draw_circle(-0.03, 0, 5, 0.3, r, g, b, 0.4, 1.6);
    draw_circle(-0.03, 0, 5, 0.2, 1.0, 1.0,1.0, 0.3, 1.7);
}



void draw_G() {
    float r = get_random_color();
    float g = get_random_color();
    float b = get_random_color();

    draw_circle(0.4, 0, 5, 0.3, r, g, b, 0.38, 1.6);
    draw_circle(0.4, 0, 5, 0.2, 1.0, 1.0, 1.0, 0.28, 1.7);
    glColor3f(r, g, b);
        glBegin(GL_POLYGON);
                glVertex2f(0.35, 0);
                glVertex2f(0.65, 0);
                glVertex2f(0.65, -0.3);
                glVertex2f(0.35,-0.3);
        glEnd();
    glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
                glVertex2f(0.45, -0.1);
                glVertex2f(0.55, -0.1);
                glVertex2f(0.55, -0.3);
                glVertex2f(0.45,-0.3);
        glEnd();
    glFlush();
}


void draw_circle(float x, float y, int step_angle_deg, float radius, float r, float g, float b, float start_comp, float end_comp) {
    glColor3f(r, g, b);
        glBegin(GL_POLYGON);
            float step_angle_rad = convert_degree_to_rad(step_angle_deg);    
            float present_angle_rad = (float)start_comp*PI;
            float x1, y1;
            while(present_angle_rad <= (float)end_comp*PI) {
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

float get_random_color() {
    float val = (float)(rand() % 100) / (float)(100);
    return val;
}