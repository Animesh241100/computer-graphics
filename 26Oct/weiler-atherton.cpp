// A simple OpenGL program for doing polygon clipping using sutherland-hodgeman algorithm

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>
#include<iostream>
#include<vector>
#include<unistd.h>

#define PI 3.14159     // Mathematical Constant PI
#define SIDE 600

using namespace std;

void draw_image();
vector<bool> get_clipped_polygon(vector<pair<int,int>> &polygon, vector<pair<int,int>> window);
vector<bool> clip_polygon(vector<pair<int,int>> polygon, pair<int,int> A, pair<int,int> B, vector<pair<int,int>> &clipped_polygon);
pair<int,int> get_intersection(pair<int,int>p11, pair<int,int>p12, pair<int,int>p21, pair<int,int>p22);
vector<pair<int,int>> get_window();
vector<pair<int,int>> get_polygon_a();
vector<pair<int,int>> get_polygon_b();
vector<pair<int,int>> get_polygon_c();
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b);
void draw_polygon_weiler(vector<pair<int,int>> P, vector<bool> is_in_out, float r, float g, float b, float r_w, float g_w, float b_w);
bool is_leftside(pair<int, int> point, pair<int,int> A, pair<int,int> B);
void print_vec(vector<pair<int,int>> p, string name);
void get_line_format(pair<int,int> p1, pair<int,int> p2, float &a, float &b, float &c);

void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);



int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Weiler Atherton Algorithm");
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
        vector<pair<int,int>> polygon = get_polygon_c();
        print_vec(polygon, "Input Polygon");
        vector<pair<int,int>> window = get_window();
        vector<bool> is_in_out = get_clipped_polygon(polygon, window);
        // print_vec(polygon, "Clipped Polygon");
        draw_polygon(window, 1.0, 0.0, 0.0);
        draw_polygon_weiler(polygon, is_in_out, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0);
        // draw_polygon(polygon, 0.0, 1.0, 0.0);
    glFlush();
}

vector<bool> get_clipped_polygon(vector<pair<int,int>> &polygon, vector<pair<int,int>> window) {
    int i = 0;
    vector<bool> is_in_out;
    for(i = 0; i < window.size(); i++){ 
        vector<pair<int,int>> clipped_polygon;
        is_in_out = clip_polygon(polygon, window[i], window[(i+1)%window.size()], clipped_polygon);
        polygon = clipped_polygon;
    }
    return is_in_out;
}

void print_vec(vector<pair<int,int>> p, string name) {
    cout << "The " << name << " vector: ";
    for(auto i : p) {
        cout << "(" << i.first << "," << i.second << "), ";
    }
    cout << endl;
}

// clips the polygon based on the algorithm
vector<bool> clip_polygon(vector<pair<int,int>> polygon, pair<int,int> A, pair<int,int> B, vector<pair<int,int>> &clipped_polygon) {
    vector<bool> is_in_out;
    for(int i = 0; i < polygon.size(); i++) {
        pair<int,int> intersection;
        bool left1 = is_leftside(polygon[i], A, B); 
        bool left2 = is_leftside(polygon[(i+1)%polygon.size()], A, B);
        if(left1 && left2) {
            is_in_out.push_back(false);
            clipped_polygon.push_back(polygon[(i+1)%polygon.size()]);
        } else if(left1 && !left2) {
            is_in_out.push_back(true);
            intersection = get_intersection(polygon[i], polygon[(i+1)%polygon.size()], A, B);
            clipped_polygon.push_back(intersection);
        }
        else if(!left1 && left2) {
            intersection = get_intersection(polygon[i], polygon[(i+1)%polygon.size()], A, B);
            is_in_out.push_back(true);
            clipped_polygon.push_back(intersection);
            is_in_out.push_back(true);
            clipped_polygon.push_back(polygon[(i+1)%polygon.size()]);
        }
    }
    return is_in_out;
}


//  returns the intersection point of the lines a and b
pair<int,int> get_intersection(pair<int,int>p11, pair<int,int>p12, pair<int,int>p21, pair<int,int>p22) {
    pair<int,int> p;
    float a1, b1, c1, a2, b2, c2;
    get_line_format(p11, p12, a1, b1, c1);
    get_line_format(p21, p22, a2, b2, c2);
    p.first = round((float)(b1*c2 - b2*c1) / (float)(a1*b2 - a2*b1)); 
    p.second = round((float)(a2*c1 - a1*c2) / (float)(a1*b2 - a2*b1)); 
    return p;
}

void get_line_format(pair<int,int> p1, pair<int,int> p2, float &a, float &b, float &c) {
    if(p2.first != p1.first) {
        a = (float)(p2.second - p1.second) / (float)(p2.first - p1.first); 
        b = -1;
        c = p1.second - a*p1.first; 
    } else {
        a = 1;
        b = 0;
        c = -p1.first;
    }
}


// returns a window polygon
vector<pair<int,int>> get_window() {
    vector<pair<int,int>> P;
    P.push_back({SIDE,0});
    P.push_back({SIDE-round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))});
    P.push_back({-SIDE+round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))});
    P.push_back({-SIDE,0});
    P.push_back({-SIDE+round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))});
    P.push_back({SIDE-round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))});
    return P;
}

// returns a polygon to be clipped
vector<pair<int,int>> get_polygon_a() {
    vector<pair<int,int>> P;
    P.push_back({500, 500});
    P.push_back({-500, 500});
    P.push_back({-500, 0});
    P.push_back({500, 0});
    return P;
}

// returns a polygon to be clipped
vector<pair<int,int>> get_polygon_b() {
    int x_off = 400;
    int y_off = -600;
    vector<pair<int,int>> P;
    P.push_back({SIDE+x_off,0+y_off});
    P.push_back({SIDE-round(SIDE*cos(PI/3))+x_off,round(SIDE*sin(PI/3))+y_off});
    P.push_back({-SIDE+round(SIDE*cos(PI/3))+x_off,round(SIDE*sin(PI/3))+y_off});
    P.push_back({-SIDE+x_off,0+y_off});
    P.push_back({-SIDE+round(SIDE*cos(PI/3))+x_off,(-1)*round(SIDE*sin(PI/3))+y_off});
    P.push_back({SIDE-round(SIDE*cos(PI/3))+x_off,(-1)*round(SIDE*sin(PI/3))+y_off});
    return P;
}

// returns a concave polygon to be clipped
vector<pair<int,int>> get_polygon_c() {
    vector<pair<int,int>> P;
    P.push_back({700, -300});
    P.push_back({700, 300});
    P.push_back({30, 300});
    P.push_back({30, 200});
    P.push_back({650, 200});
    P.push_back({650, -200});
    P.push_back({30, -200});
    P.push_back({30, -300});
    return P;
}


// draws the polygon P
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b) {
    glColor3f(r, g, b);
    for(int i = 0; i < P.size(); i++){ 
        draw_line(P[i], P[(i+1)%P.size()]);
    }
}

void draw_polygon_weiler(vector<pair<int,int>> P, vector<bool> is_in_out, float r, float g, float b, float r_w, float g_w, float b_w) {
    for(int i = 0; i < P.size(); i++){ 
        if(is_in_out[(i+1)%P.size()])
            glColor3f(r_w, g_w, b_w);
        else
            glColor3f(r, g, b);
        draw_line(P[i], P[(i+1)%P.size()]);
    }
}

// returns true if the point is on the left side of 'e' edge
bool is_leftside(pair<int, int> point, pair<int,int> A, pair<int,int> B) {
    return ((B.first - A.first)*(point.second - A.second) - (B.second - A.second)*(point.first - A.first)) > 0;
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
