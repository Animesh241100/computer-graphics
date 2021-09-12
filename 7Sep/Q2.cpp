// A simple OpenGL program that draws an arc from angle alpha to beta.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

#define PI 3.14159     // Mathematical Constant PI
#define EPSILON 0.01f

using namespace std;

float alpha, beta;

void draw_arc();
void draw_circular_arc(float cx, float cy,  float r);
void plot_octal(float x1, float y1, float cx, float cy);
void plot_point(float x, float y);
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);

int main(int argc, char **argv) {
    cout << "Enter alpha and beta in degrees: " << endl;
    cin >> alpha >> beta;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - An arc");
    glutDisplayFunc(draw_arc);
    glutMainLoop();
    return 0;
}

void draw_arc() {
    alpha = (float)alpha * (PI) / (float)180;
    beta = (float)beta * (PI) / (float)180;
    float radius = 500;
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_circular_arc(0, 0, radius);
        draw_line({0,0},{radius*cos(alpha), radius*sin(alpha)});
        draw_line({0,0},{radius*cos(alpha+beta), radius*sin(alpha+beta)});
    glFlush();
}



/************************ Drawing utility functions *************************/
void draw_circular_arc(float cx, float cy,  float r) {
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
        plot_point(x1 + cx, y1 + cy);
        plot_point(y1 + cx, x1 + cy);
        plot_point(y1 + cx, -x1 + cy);
        plot_point(x1 + cx, -y1 + cy);
        plot_point(-x1 + cx, -y1 + cy);
        plot_point(-y1 + cx, -x1 + cy);
        plot_point(-y1 + cx, x1 + cy);
        plot_point(-x1 + cx, y1 + cy);
    glEnd();
}


void plot_point(float x, float y) {
    float slope = atan2(y, x);
    if(slope < 0)
        slope += 2*PI;
    bool flip_arc = false;
    float slope1 = alpha >= 0 ? alpha : 2*PI + alpha;
    float slope2 = alpha + beta >= 0 ? alpha + beta : 2*PI + alpha + beta;
    if(slope2 < 0) {
        slope2 = slope2 + 2*PI;
        flip_arc = true;
    }
    if(alpha < 0 && beta > 0) {
        flip_arc = slope2 > slope1 ? false : true;
    }
    if(alpha > 0 && beta < 0 && slope1 < slope2)
        flip_arc = true;
    if(slope1 > slope2)
        swap(slope1, slope2);
    bool cond1 = (slope >= slope1 && slope <= slope2) || (slope2 > 2*PI && slope >= slope1 || slope <= slope2-2*PI);
    if((!flip_arc && cond1) || (flip_arc && !cond1) || fabs(abs(beta) - 2*PI) <= EPSILON)
        glVertex2i(x, y);
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