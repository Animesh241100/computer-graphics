// A simple OpenGL program that draws a house.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>

#define PI 3.14159     // Mathematical Constant PI

using namespace std;

void draw_square();
void draw_triangle();
void draw_house();
void draw_right_window();
void draw_left_window();
void draw_door();
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair(pair<float,float> &curr_pair, float m, bool decrement) ;


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
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glOrtho(-250, 250, -250, 250, -250, 250);
        draw_square();
        draw_triangle();
        draw_right_window();
        draw_left_window();
        draw_door();
    glFlush();
}

void draw_square() {
    glColor3f(0.0, 0.3, 0.9);
        for(int i = 0; i >= -200; i--)
            draw_line({-100, i}, {100, i});
    glFlush();
}

void draw_triangle() {
    glColor3f(0.8, 0.2, 0.0);
    float j = 0;
    for(int i = 170; i >= 0; i--){
        draw_line({(-1)*round(j), i}, {round(j), i});
        j = j + (float)6/(float)10;
    }
    glFlush();
}

void draw_right_window() {
    glColor3f(1.0, 1.0, 1.0);
        for(int i = -80; i >= -110; i--)
            draw_line({40, i}, {70, i});
    glFlush();
}

void draw_left_window() {
    glColor3f(1.0, 1.0, 1.0);
        for(int i = -80; i >= -110; i--)
            draw_line({-40, i}, {-70, i});
    glFlush();
}

void draw_door() {
    glColor3f(1.0, 1.0, 1.0);
        for(int i = -110; i >= -198; i--)
            draw_line({-20, i}, {20, i});
    glFlush();
}




//draws a line from `src` co-ordinate to `dest` co-ordinate
void draw_line(pair<float,float> src, pair<float,float> dest) {
    float m = (float)(dest.second - src.second) / (float)(dest.first - src.first);
    bool decrement = false;
    if(abs(m) >= 1)
        decrement = dest.second >= src.second ? false : true;
    else
        decrement = dest.first >= src.first ? false : true;
    glPointSize(3);
    pair<float,float> temp = src;
    while(1) {
        if(round(temp.first) == round(dest.first) && round(temp.second) == round(dest.second))
            break;
        glBegin(GL_POINTS);
            glVertex2i(round(temp.first), round(temp.second));
        glEnd();
        get_next_pair(temp, m, decrement);
    }
}


// modifies the curr_pair to the next co-ordinate pair value
void get_next_pair(pair<float,float> &curr_pair, float m, bool decrement) {
    int inc = 1;
    if(decrement) {
        inc = (-1)*inc;
        m = (-1)*m;
    }
    if(abs(m) >= 1) {
        curr_pair.second = curr_pair.second + inc;
        curr_pair.first = curr_pair.first + (float)1 / m;
    } else {
        curr_pair.first = curr_pair.first + inc;
        curr_pair.second = curr_pair.second + m; 
    }
}