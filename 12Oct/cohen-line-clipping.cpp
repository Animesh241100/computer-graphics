// A simple OpenGL program for doing line clipping using cohen sutherland algorithm

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

#define NUM_LINES 1000
#define L_CODE 1
#define R_CODE 2
#define B_CODE 4
#define T_CODE 8


using namespace std;

float x_max, x_min, y_max, y_min;

void draw_image();
void draw_clipped_lines();
void AcceptOrRejectLine(pair<int, int> A, pair<int, int> B);
pair<pair<int, int>, pair<int, int>> get_random_line();
void draw_window();
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);
int get_region_code(pair<int, int> P);

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen Sutherland Line Clipping");
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

// using cohen-sutherland algorithm
void AcceptOrRejectLine(pair<int, int> A, pair<int, int> B) {
    int x1 = A.first;
    int y1 = A.second;
    int x2 = B.first;
    int y2 = B.second;
    while(1) {
        int code_1 = get_region_code({x1, y1});
        int code_2 = get_region_code({x2, y2});

        if((code_1|code_2) == 0)  { // completely inside
            draw_line({x1, y1}, {x2, y2});
            break;
        }
        else if((code_1&code_2) != 0)  // completely outside  
            break;
        else  { // clipping is required
            int temp_code, x, y;
            temp_code = code_1 != 0 ? code_1 : code_2;
            if(temp_code & L_CODE)  {
                x = x_min;
                y = (int)((float)y1 + (float)(y2-y1)*((float)(x_min-x1)/(x2-x1)));
            }
            if(temp_code & R_CODE)  {
                x = x_max;
                y = (int)((float)y1 + (float)(y2-y1)*((float)(x_max-x1)/(x2-x1)));
            }
            if(temp_code & B_CODE)  {
                y = y_min;
                x = (int)((float)x1 + (float)(x2-x1)*((float)(y_min-y1)/(y2-y1)));
            }
            if(temp_code & T_CODE)  {
                y = y_max;
                x = (int)((float)x1 + (float)(x2-x1)*((float)(y_max-y1)/(y2-y1)));
            }
            if(temp_code == code_1) { // clipping step
                x1 = x;
                y1 = y;
            }
            else {              // clipping step
                x2 = x;
                y2 = y;
            }
        }
    }
}


int get_region_code(pair<int, int> P) {
    int code = 0;
    if(P.first < x_min)
        code = code | L_CODE;
    if(P.first > x_max)
        code = code | R_CODE;
    if(P.second < y_min)
        code = code | B_CODE;
    if(P.second > y_max)
        code = code | T_CODE;
    return code;
}


pair<pair<int, int>, pair<int, int>> get_random_line() {
    pair<int, int> p1, p2;
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
