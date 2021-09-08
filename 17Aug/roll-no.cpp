// A simple OpenGL program that draws a polygon.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#define PI 3.14159     // Mathematical Constant PI

void draw_roll_no();
void draw_6();
void draw_5();

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
    glutDisplayFunc(draw_roll_no);
    glutMainLoop();
    return 0;
}


void draw_roll_no() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        draw_circle(-0.6, 0, 5, 0.3, 0.0, 0.0, 0.0, 0, 2);
        draw_circle(-0.6, 0, 5, 0.2, 1.0, 1.0, 1.0, 0, 2);

        draw_6();
        draw_5();
    glFlush();
}

void draw_6() {
    float r = get_random_color();
    float g = get_random_color();
    float b = get_random_color();
    draw_circle(-0.05, 0, 5, 0.3, r, g, b, 0, 2);
    draw_circle(-0.05, 0, 5, 0.2, 1.0, 1.0, 1.0, 0, 2);
    glColor3f(r, g, b);
        glBegin(GL_QUADS);
            glVertex2f(-0.20, 0.13);
            glVertex2f(0.08, 0.6);
            glVertex2f(0.06, 0.7);
            glVertex2f(-0.32, 0.10);
        glEnd();
    glFlush();
}


void draw_5() {
    float r = get_random_color();
    float g = get_random_color();
    float b = get_random_color();

    draw_circle(0.4, 0, 5, 0.3, r, g, b, 1.3, 2.5);
    draw_circle(0.4, 0, 5, 0.2, 1.0, 1.0, 1.0, 1.2, 2.7);
    glColor3f(r, g, b);
        glBegin(GL_POLYGON);
            glVertex2f(0.4, 0.2);
            glVertex2f(0.3, 0.2);
            glVertex2f(0.43, 0.63);
            glVertex2f(0.53, 0.63);
        glEnd();
    glColor3f(r, g, b);
        glBegin(GL_POLYGON);
            glVertex2f(0.43, 0.63);
            glVertex2f(0.43, 0.55);
            glVertex2f(0.7, 0.57);
            glVertex2f(0.7, 0.65);
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