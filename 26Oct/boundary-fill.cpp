// A simple OpenGL implementation of boudnary filling algorithm

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

#define PI 3.14159     // Mathematical Constant PI
#define SIDE 100
#define MAX 200

using namespace std;


vector<vector<bool>> is_colored(402, vector<bool>(402, false));

void draw_image();
void plot_point(int x1, int y1);
void boundary_fill(pair<int,int> seed);
vector<pair<int,int>> get_boundary();
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b);


void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Boundary Fill Algorithm");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glOrtho(-MAX, MAX, -MAX, MAX, -MAX, MAX);
        vector<pair<int,int>> boundary = get_boundary();
        draw_polygon(boundary, 0.0, 0.0, 1.0);
        glFlush();
        // sleep(1);
        boundary_fill({0, 0});
    glFlush();
}

void boundary_fill(pair<int,int> seed) {
    // usleep(10);
    if(seed.first > MAX || seed.first < -MAX || seed.second > MAX || seed.second < -MAX)
        return;
    if(is_colored[seed.first+MAX][seed.second+MAX])
        return;
    glColor3f(1.0, 0.0, 0.0);
    plot_point(seed.first, seed.second);
    boundary_fill({seed.first+1, seed.second});
    boundary_fill({seed.first, seed.second+1});
    boundary_fill({seed.first-1, seed.second});
    boundary_fill({seed.first, seed.second-1});
}

// returns a window polygon
vector<pair<int,int>> get_boundary() {
    vector<pair<int,int>> P;
    P.push_back({SIDE,0});
    P.push_back({SIDE-round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))});
    P.push_back({-SIDE+round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))});
    P.push_back({-SIDE,0});
    P.push_back({-SIDE+round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))});
    P.push_back({SIDE-round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))});
    return P;
}

// draws the polygon P
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b) {
    glColor3f(r, g, b);
    for(int i = 0; i < P.size(); i++){ 
        draw_line(P[i], P[(i+1)%P.size()]);
    }
}



/********************************* Line Drawing Utility ******************************/

//draws a line from `src` co-ordinate to `dest` co-ordinate (BLD)
void draw_line(pair<float,float> src, pair<float,float> dest) {
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
    plot_point(round(src.first), round(src.second));
    plot_point(round(dest.first), round(dest.second));
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
            plot_point(x1, y1);
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

void plot_point(int x1, int y1) {
    glPointSize(2);
    // glColor3f(r, g, b);
    glBegin(GL_POINTS);
        glVertex2i(x1, y1);
        is_colored[x1+MAX][y1+MAX] = true;
    glEnd();
}