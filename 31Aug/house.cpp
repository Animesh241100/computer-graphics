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
void get_next_pair_dda(pair<float,float> &curr_pair, float m) ;
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);
pair<float,float> get_next_pair_brute(pair<float,float> &curr_pair, pair<float,float> src, pair<float,float> dest);


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
    if(m < 0) {
        src.first = (-1) * src.first;
        dest.first = (-1) * dest.first;
    }
    float abs_m = abs(m);
    if(abs_m > 1) {
        swap(src.first, src.second);
        swap(dest.first, dest.second);
        m = (float)1 / m;
    }
    if(src.first > dest.first) 
        src.swap(dest);
    float dx = dest.first - src.first;
    float dy = dest.second - src.second;
    float val1 = 2 * dy;
    float temp_dp = 2 * dy - dx;
    float val2 = 2 * dy - 2 * dx;
    glPointSize(3);
    pair<float,float> temp = src;
    while(1) {
        if(round(temp.first) == round(dest.first) && round(temp.second) == round(dest.second))
            break;
        glBegin(GL_POINTS);
            int x1 = round(temp.first);
            int y1 = round(temp.second);
            if(abs_m > 1){
                x1 = round(temp.second);
                y1 = round(temp.first);
            }
            if(m < 0)
                x1 = (-1) * x1;
            glVertex2i(x1, y1);
        glEnd();
        // get_next_pair_bld(temp, temp_dp, val1, val2);
        get_next_pair_dda(temp, m);
        // temp = get_next_pair_brute(temp, src, dest);
    }
}

// using brasenhem algorithm
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2) {
    curr_pair.first = curr_pair.first + 1;
    if(temp_dp >= 0) {
        curr_pair.second = curr_pair.second + 1;
        temp_dp = temp_dp + val2;
    } else {
        temp_dp = temp_dp + val1;
    }
}


// using DDA algorithm
void get_next_pair_dda(pair<float,float> &curr_pair, float m) {
    curr_pair.first = curr_pair.first + 1;
    curr_pair.second = curr_pair.second + abs(m); 
}

// using brute force
pair<float,float> get_next_pair_brute(pair<float,float> &curr_pair, pair<float,float> src, pair<float,float> dest) {
    pair<float,float> next_pair;
    next_pair.first = curr_pair.first + 1;
    next_pair.second = (next_pair.first - src.first) * (float)(dest.second - src.second) / (float)(dest.first - src.first);
    next_pair.second += src.second;
    return next_pair;
}