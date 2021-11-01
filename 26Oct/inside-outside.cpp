// A simple OpenGL program for doing line clipping using liang baskey algorithm

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<vector>
#include<unistd.h>

#define PI 3.14159     // Mathematical Constant PI
#define SIDE 1000

using namespace std;

void draw_image();
void draw_polygon_point();
vector<pair<int,int>> draw_polygon();
bool is_inside(vector<pair<int,int>> P, pair<int,int> point);
bool is_leftside(pair<int, int> point, pair<int,int> A, pair<int,int> B);

void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);



int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Checking inside outside");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

void draw_image() {
    srand(time(0));
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_polygon_point();
    glFlush();
}

void draw_polygon_point() {
    vector<pair<int,int>> P = draw_polygon();
    int x1 = rand() % 2000 - 1000;
    int y1 = rand() % 2000 - 1000;
    glPointSize(4);
    glBegin(GL_POINTS);
    if(is_inside(P, {x1, y1})) {
        glColor3f(0.0, 1.0, 0.0);
        cout << "Inside\n";
    }
    else   
        cout << "Outside\n";
    cout << x1 << " " << y1 << endl;
    glVertex2f(x1, y1);
    glEnd();
}

vector<pair<int,int>> draw_polygon() {
    vector<pair<int,int>> P;
    P.push_back({SIDE,0});
    P.push_back({SIDE-round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))});
    P.push_back({-SIDE+round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))});
    P.push_back({-SIDE,0});
    P.push_back({-SIDE+round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))});
    P.push_back({SIDE-round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))});
    for(int i = 0; i < P.size(); i++) 
        draw_line(P[i], P[(i+1)%P.size()]);
    return P;
}

// returns true if the point is inside the polygon P
bool is_inside(vector<pair<int,int>> P, pair<int,int> point) {
    for(int i = 0; i < P.size(); i++) {
        if(!is_leftside(point, P[i], P[(i+1)%P.size()]))
            return false;
    }
    return true;
}

// returns true if the point is on the left side of 'e' edge
bool is_leftside(pair<int, int> point, pair<int,int> A, pair<int,int> B) {
    return ((B.first - A.first)*(point.second - A.second) - (B.second - A.second)*(point.first - A.first)) > 0;
}


/********************************* Line Drawing Utility ******************************/

//draws a line from `src` co-ordinate to `dest` co-ordinate (BLD)
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
