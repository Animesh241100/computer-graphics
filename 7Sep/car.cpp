// A simple OpenGL program that draws a car.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

using namespace std;

void draw_car();
void draw_box();
void draw_window_right();
void draw_window_left();
void draw_front_box();

void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);

void draw_circle(float cx, float cy,  float r);
void plot_octal(float x1, float y1, float cx, float cy);

int main(int argc, char **argv) {
    srand(time(0));
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
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_front_box();
        draw_box();
        draw_circle(570, -30, 130);
        draw_circle(-520, -30, 130);
        draw_window_left();
        draw_window_right();
    glFlush();
}

void draw_window_left() {
    draw_line({-400, 170}, {-20, 370});
    draw_line({-20, 370}, {420, 360});
    draw_line({420, 360}, {400, 170});
    draw_line({400, 170}, {-400, 170});
}

void draw_window_right() {
    draw_line({430, 170}, {450, 350});
    draw_line({450, 350}, {700, 260});
    draw_line({700, 260}, {730, 170});
    draw_line({730, 170}, {430, 170});
}

void draw_front_box() {
    draw_line({-800, 120}, {-800, 170});
    draw_line({-800, 170}, {-450, 180});
}


void draw_box() {  
    draw_line({-20, 400},{500, 380});
    draw_line({500, 380},{700, 300});
    draw_line({700, 300},{800, 120});
    // draw_line({800, 120},{-550, 120});
    draw_line({-450, 180},{-20, 400});
    draw_line({800, 120},{800, -40});
    draw_line({800, -40},{-800, -40});
    draw_line({800, -40},{800, 120});
}




/************************ Drawing utility functions *************************/
void draw_circle(float cx, float cy,  float r) {
    float x_i = 0;
    float y_i = r;
    float p_i = 1 - r;
    while(y_i >= x_i) {
        x_i++;
        if(p_i < 0)
            p_i += 2*x_i + 1;    
        else {
            y_i--;
            p_i += 2*x_i - 2*y_i + 1;
        }
        plot_octal(x_i, y_i, cx, cy);
    }
}

void plot_octal(float x1, float y1, float cx, float cy) {
    glPointSize(2);
    glBegin(GL_POINTS);
        // glVertex2i(x1 + cx, y1 + cy);
        // glVertex2i(y1 + cx, x1 + cy);
        // glVertex2i(x1 + cx, -y1 + cy);
        // glVertex2i(-y1 + cx, x1 + cy);
        // glVertex2i(-x1 + cx, y1 + cy);
        // glVertex2i(y1 + cx, -x1 + cy);
        // glVertex2i(-x1 + cx, -y1 + cy);
        // glVertex2i(-y1 + cx, -x1 + cy);
        draw_line({x1 + cx, y1 + cy},{-x1 + cx, y1 + cy});
        draw_line({y1 + cx, x1 + cy},{-y1 + cx, x1 + cy});
        draw_line({x1 + cx, -y1 + cy},{-x1 + cx, -y1 + cy});
        draw_line({y1 + cx, -x1 + cy},{-y1 + cx, -x1 + cy});
    glEnd();
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
    glPointSize(2);
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

/**********************************************************************************/