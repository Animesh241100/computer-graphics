// A simple OpenGL implementation of flood fill algorithm

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<vector>
#include<map>
#include<unistd.h>
#include<algorithm>

#define SIDE 100
#define MAX 200
#define PREV_COLOR 1
#define NEW_COLOR 2

using namespace std;


vector<vector<int>> color(402, vector<int>(402, 0));

void draw_image();
void draw_region();
void plot_point(int x1, int y1, int color_code);
void flood_fill(pair<int,int> seed);

void draw_line(pair<float,float> src, pair<float,float> dest, int color_code);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Flood Fill Algorithm");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glOrtho(-MAX, MAX, -MAX, MAX, -MAX, MAX);
        draw_region();
        glFlush();
        // sleep(1);
        flood_fill({0, 0});
    glFlush();
}

void flood_fill(pair<int,int> seed) {
    usleep(100);
    if(seed.first > MAX || seed.first < -MAX || seed.second > MAX || seed.second < -MAX)
        return;
    if(color[seed.first+MAX][seed.second+MAX] != PREV_COLOR)
        return;
    glColor3f(0.0, 1.0, 0.0);
    plot_point(seed.first, seed.second, NEW_COLOR);
    flood_fill({seed.first+1, seed.second});
    flood_fill({seed.first, seed.second+1});
    flood_fill({seed.first-1, seed.second});
    flood_fill({seed.first, seed.second-1});
}

// draws a random pre-existing region
void draw_region() {
    glColor3f(1.0, 0.0, 0.1);
    int i = 180;
    for(int i = 180; i >= 50; i--) {
        draw_line({i, i}, {-i, i}, PREV_COLOR);
        draw_line({i, -i}, {-i, -i}, PREV_COLOR);
    }
    for(int i = 50; i >= -50; i--) {
        draw_line({30,i}, {-30, i}, PREV_COLOR);
    }
}


/********************************* Line Drawing Utility ******************************/

//draws a line from `src` co-ordinate to `dest` co-ordinate (BLD)
void draw_line(pair<float,float> src, pair<float,float> dest, int color_code) {
    if(src.first == dest.first && src.second > dest.second)
        src.swap(dest);
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
    pair<float,float> temp = src;
    plot_point(round(src.first), round(src.second), color_code);
    plot_point(round(dest.first), round(dest.second), color_code);
    while(1) {
        if(round(temp.first) == round(dest.first) && round(temp.second) == round(dest.second))
            break;
            int x1 = round(temp.first);
            int y1 = round(temp.second);
            if(abs_m > 1){
                x1 = round(temp.second);
                y1 = round(temp.first);
            }
            if(m < 0)
                x1 = (-1) * x1;
            plot_point(x1, y1, color_code);
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

// plots a point of the given color code
void plot_point(int x1, int y1, int color_code) {
    glPointSize(2);
    // glColor3f(r, g, b);
    glBegin(GL_POINTS);
        glVertex2i(x1, y1);
        color[x1+MAX][y1+MAX] = color_code;
    glEnd();
}