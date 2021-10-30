// A simple OpenGL program for doing line clipping using liang baskey algorithm

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

#define NUM_LINES 1000

using namespace std;

float x_max, x_min, y_max, y_min;

void draw_image();
void draw_clipped_lines();
void AcceptOrRejectLine(pair<float, float> A, pair<float, float> B);
pair<pair<float, float>, pair<float, float>> get_random_line();
void draw_window();
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Liang Barskey Line Clipping");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    x_max = 700;
    x_min = -700;
    y_max = 500;
    y_min = -500;
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_clipped_lines();
    glFlush();
}

/************************ Drawing utility functions *************************/
void draw_clipped_lines() {
    draw_window();
    pair<pair<float, float>, pair<float, float>> Line;
    glColor3f(0.0, 1.0, 0.0);
    for(int i = 0 ; i < NUM_LINES; i++) {
        Line = get_random_line();
        AcceptOrRejectLine(Line.first, Line.second);
    }
}


void AcceptOrRejectLine(pair<float, float> A, pair<float, float> B) {
    float dx = B.first - A.first;
    float dy = B.second - A.second;
    pair<float, float> p1, p2;
    float u_max = 0; 
    float u_min = 1;
    float p[4], q[4];
    q[0] = A.first - x_min;
    q[1] = x_max - A.first;
    q[2] = A.second - y_min;
    q[3] = y_max - A.second;
    p[0] = -dx;
    p[1] = dx;
    p[2] = -dy;
    p[3] = dy;
    for(int i = 0; i < 4; i++) {
        if(p[i] == 0 && q[i] < 0)
            return;
        float val = (float)q[i] / (float)p[i];
        if(p[i] < 0)
            u_max = max(val, u_max);
        else if(p[i] >= 0)   
            u_min = min(val, u_min);
    }
    if(u_max <= u_min) {
        p1 = {round(A.first + u_max*(float)dx), round(A.second + u_max*(float)dy)};
        p2 = {round(A.first + u_min*(float)dx), round(A.second + u_min*(float)dy)};
        draw_line(p1, p2);
    }
}


pair<pair<float, float>, pair<float, float>> get_random_line() {
    pair<float, float> p1, p2;
    p1.first = rand() % 2000 - 1000;
    p1.second = rand() % 2000 - 1000;
    p2.first = rand() % 2000 - 1000;
    p2.second = rand() % 2000 - 1000;
    return {p1, p2};
}

void draw_window() {
    draw_line({x_max, y_min}, {x_max, y_max});
    draw_line({x_max, y_min}, {x_min, y_min});
    draw_line({x_min, y_min}, {x_min, y_max});
    draw_line({x_min, y_max}, {x_max, y_max});
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
