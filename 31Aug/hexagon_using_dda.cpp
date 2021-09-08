// A simple OpenGL program that draws a hexagon.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<unistd.h>

#define PI 3.14159     // Mathematical Constant PI

using namespace std;

int num_points = 0;
double mean_square_error = 0;

void draw_hexagon();
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_dda(pair<float,float> &curr_pair, float m) ;
pair<float,float> get_next_pair_brute(pair<float,float> &curr_pair, pair<float,float> src, pair<float,float> dest);

int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a Hexagon");
    glutDisplayFunc(draw_hexagon);
    glutMainLoop();
    return 0;
}

// Draws a hexagon
void draw_hexagon() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glOrtho(-250, 250, -250, 250, -250, 250);
    draw_line({200,0},{200-round(200*cos(PI/3)),round(200*sin(PI/3))});
    draw_line({200,0},{200-round(200*cos(PI/3)),(-1)*round(200*sin(PI/3))});
    draw_line({-200,0},{-200+round(200*cos(PI/3)),round(200*sin(PI/3))});
    draw_line({-200,0},{-200+round(200*cos(PI/3)),(-1)*round(200*sin(PI/3))});

    draw_line({200-round(200*cos(PI/3)),(-1)*round(200*sin(PI/3))}, {-200+round(200*cos(PI/3)),(-1)*round(200*sin(PI/3))});
    draw_line({200-round(200*cos(PI/3)),round(200*sin(PI/3))}, {-200+round(200*cos(PI/3)),round(200*sin(PI/3))});
    cout << "Total Number of points: " << num_points << endl;
    cout << "Mean Square Error: " << (double)mean_square_error / (double)num_points << endl;
    
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
        pair<float,float>brute_next = get_next_pair_brute(temp, src, dest);
        get_next_pair_dda(temp, m);
        mean_square_error += pow((brute_next.second - round(temp.second)), 2);
        num_points++;
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