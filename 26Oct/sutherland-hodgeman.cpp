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
#define SIDE 700

using namespace std;

void draw_image();
vector<pair<int,int>> get_clipped_polygon(vector<pair<int,int>> polygon, vector<pair<int,int>> window);
void clip_polygon(vector<pair<int,int>> polygon, pair<int,int> A, pair<int,int> B, vector<pair<int,int>> &clipped_polygon);
pair<int,int> get_intersection(pair<int,int>p11, pair<int,int>p12, pair<int,int>p21, pair<int,int>p22);
vector<pair<int,int>> get_window();
vector<pair<int,int>> get_polygon();
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b);
bool is_leftside(pair<int, int> point, pair<int,int> A, pair<int,int> B);
void print_vec(vector<pair<int,int>> p);
void print_pair(pair<int,int> p, string name);
void get_line_format(pair<int,int> p1, pair<int,int> p2, float &a, float &b, float &c);

void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);



int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sutherland Hodgeman Algorithm");
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
        vector<pair<int,int>> polygon = get_polygon();
        vector<pair<int,int>> window = get_window();
        vector<pair<int,int>> clipped_polygon = get_clipped_polygon(polygon, window);
        // draw_polygon(polygon, 0.0, 1.0, 0.0);
        // draw_line({800,700}, {-800,700});
        // draw_line({-800,-700}, {-800,700});
        // draw_line({-800,-700}, {800,700});
        draw_polygon(window, 1.0, 0.0, 0.0);
        draw_polygon(clipped_polygon, 0.0, 1.0, 0.0);

        // draw_line({500, 0}, {500, 500});
        // draw_line({700, 0}, {350, 606});
        // pair<int,int> p = get_intersection({500, 0}, {500, 500}, {700, 0}, {350, 606});
        
        // glPointSize(4);
        // glBegin(GL_POINTS);
        // glColor3f(0.0,0.0,0.0);
        // glVertex2f(p.first, p.second);
        // glEnd();

    glFlush();
}

vector<pair<int,int>> get_clipped_polygon(vector<pair<int,int>> polygon, vector<pair<int,int>> window) {
    vector<pair<int,int>> clipped_polygon;
    int i = 0;
    for(i = 0; i < window.size(); i++){ 
        clip_polygon(polygon, window[i], window[(i+1)%window.size()], clipped_polygon);
        // print_vec(clipped_polygon);
    }
    return clipped_polygon;
}

void print_vec(vector<pair<int,int>> p) {
    for(auto i : p) {
        cout << "(" << i.first << "," << i.second << "), ";
    }
    cout << endl;
}

// clips the polygon based on the algorithm
void clip_polygon(vector<pair<int,int>> polygon, pair<int,int> A, pair<int,int> B, vector<pair<int,int>> &clipped_polygon) {
    for(int i = 0; i < polygon.size(); i++) {
        pair<int,int> intersection;
        bool left1 = is_leftside(polygon[i], A, B); 
        bool left2 = is_leftside(polygon[(i+1)%polygon.size()], A, B);
        if(left1 && left2){
            print_pair(polygon[(i+1)%polygon.size()], "0 " + to_string(i));
            clipped_polygon.push_back(polygon[(i+1)%polygon.size()]);
        }
        else if(left1 && !left2) {
            intersection = get_intersection(polygon[i], polygon[(i+1)%polygon.size()], A, B);
            print_pair(intersection, "1 " + to_string(i));
            clipped_polygon.push_back(intersection);
        }
        else if(!left1 && left2) {
            intersection = get_intersection(polygon[i], polygon[(i+1)%polygon.size()], A, B);
            print_pair(intersection, "2 " + to_string(i));
            clipped_polygon.push_back(intersection);
            print_pair(polygon[(i+1)%polygon.size()], "2 " + to_string(i));
            clipped_polygon.push_back(polygon[(i+1)%polygon.size()]);
        }
        // print_vec(clipped_polygon);
    }
}

void print_pair(pair<int,int> p, string name) {
    cout << name << " " << p.first << ", " << p.second << endl;
}

//  returns the intersection point of the lines a and b
pair<int,int> get_intersection(pair<int,int>p11, pair<int,int>p12, pair<int,int>p21, pair<int,int>p22) {
    pair<int,int> p;
    float a1, b1, c1, a2, b2, c2;
    get_line_format(p11, p12, a1, b1, c1);
    get_line_format(p21, p22, a2, b2, c2);
    cout << a1 << " " << b1 << " " << c1 << endl;
    cout << a2 << " " << b2 << " " << c2 << endl;
    p.first = round((float)(b1*c2 - b2*c1) / (float)(a1*b2 - a2*b1)); 
    p.second = round((float)(a2*c1 - a1*c2) / (float)(a1*b2 - a2*b1)); 
    print_pair(p11, "p11");
    print_pair(p12, "p12");
    print_pair(p21, "p21");
    print_pair(p22, "p22");
    print_pair(p, "answer ");
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
vector<pair<int,int>> get_polygon() {
    vector<pair<int,int>> P;
    P.push_back({500, 500});
    P.push_back({-500, 500});
    P.push_back({-500, 0});
    P.push_back({500, 0});
    return P;
}

// draws the polygon P
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b) {
    glColor3f(r, g, b);
    for(int i = 0; i < P.size(); i++){ 
        // cout << P[i].first << "," << P[i].second << "; " << P[(i+1)%P.size()].first << "," << P[(i+1)%P.size()].second << endl;
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
    if(src.first == dest.first && src.first < 0)
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
