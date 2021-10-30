// A simple OpenGL program for doing point clipping

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

#define NUM_POINTS 10000

using namespace std;

void draw_image();
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);
void draw_points(float x_max, float x_min, float y_max, float y_min);
bool is_valid(pair<float, float> p, float x_max, float x_min, float y_max, float y_min);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Point Clipping");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    float x_max = 700;
    float x_min = -700;
    float y_max = 500;
    float y_min = -500;
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_points(x_max, x_min, y_max, y_min);
    glFlush();
}

/************************ Drawing utility functions *************************/
void draw_points(float x_max, float x_min, float y_max, float y_min) {
    draw_line({x_max, y_min}, {x_max, y_max});
    draw_line({x_max, y_min}, {x_min, y_min});
    draw_line({x_min, y_min}, {x_min, y_max});
    draw_line({x_min, y_max}, {x_max, y_max});

    glPointSize(3);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < NUM_POINTS; i++) {
        float x = rand() % 2000 - 1000;
        float y = rand() % 2000 - 1000;
        glColor3f(1.0, 0.0, 0.0);
        if(is_valid({x, y}, x_max, x_min, y_max, y_min))
            glVertex2i(x, y);
    }
    glEnd();
}

bool is_valid(pair<float, float> p, float x_max, float x_min, float y_max, float y_min) {
    return (p.first >= x_min && p.first <= x_max && p.second >= y_min && p.second <= y_max);
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
    glPointSize(1);
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
        get_next_pair_bld(temp, temp_dp, val1, val2);
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
