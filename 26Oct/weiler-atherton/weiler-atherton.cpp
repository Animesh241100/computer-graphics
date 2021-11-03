// A simple OpenGL program for doing polygon clipping using weiler-atherton algorithm

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
#define INSIDE 1
#define OUTSIDE 2
#define IN_OUT 3
#define OUT_IN 4
#define NONE 5

using namespace std;

class Vertex {
  public:
    int x;
    int y;
    int type;
    Vertex() {
        x = 0;
        y = 0;
        type = NONE; 
    }
    Vertex(int x_, int y_, int t_) {
        x = x_;
        y = y_;
        type = t_;
    }
};

void draw_image();
void get_clipped_polygon(vector<Vertex> &polygon, vector<pair<int,int>> window);
void clip_polygon(vector<Vertex> polygon, pair<int,int> A, pair<int,int> B, vector<Vertex> &clipped_polygon);
pair<int,int> get_intersection(pair<int,int>p11, pair<int,int>p12, pair<int,int>p21, pair<int,int>p22);
vector<pair<int,int>> get_window();
vector<Vertex> get_polygon_a();
vector<Vertex> get_polygon_c();
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b);
void draw_polygon_weiler(vector<Vertex> P, float r, float g, float b);
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
    glutCreateWindow("Weiler Atherton's Algorithm");
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
        vector<Vertex> polygon = get_polygon_c();
        // print_vec(polygon, "Input Polygon");
        vector<pair<int,int>> window = get_window();
        get_clipped_polygon(polygon, window);
        // print_vec(polygon, "Clipped Polygon");
        // draw_polygon(polygon, 0.0, 1.0, 0.0);
        draw_polygon(window, 1.0, 0.0, 0.0);
        draw_polygon_weiler(polygon, 0.0, 1.0, 0.0);
    glFlush();
}

void get_clipped_polygon(vector<Vertex> &polygon, vector<pair<int,int>> window) {
    int i = 0;
    for(i = 0; i < window.size(); i++){ 
        vector<Vertex> clipped_polygon;
        clip_polygon(polygon, window[i], window[(i+1)%window.size()], clipped_polygon);
        polygon = clipped_polygon;
    }
}

void print_vec(vector<Vertex> p, string name) {
    cout << "The " << name << " vector: ";
    for(auto i : p) {
        cout << "(" << i.x << "," << i.y << "," << i.type << "), ";
    }
    cout << endl;
}

// clips the polygon based on the algorithm
void clip_polygon(vector<Vertex> polygon, pair<int,int> A, pair<int,int> B, vector<Vertex> &clipped_polygon) {
    for(int i = 0; i < polygon.size(); i++) {
        Vertex intersection;
        bool left1 = is_leftside({polygon[i].x, polygon[i].y}, A, B); 
        bool left2 = is_leftside({polygon[(i+1)%polygon.size()].x, polygon[(i+1)%polygon.size()].y}, A, B);
        if(left1 && left2) {
            // polygon[(i+1)%polygon.size()].type = INSIDE;
            clipped_polygon.push_back(polygon[(i+1)%polygon.size()]);
        }
        else if(left1 && !left2) {
            bool flag = false;
            pair<int,int> p = get_intersection({polygon[i].x, polygon[i].y}, {polygon[(i+1)%polygon.size()].x, polygon[(i+1)%polygon.size()].y}, A, B);
            for(int i = 0; i < clipped_polygon.size();i++) {
                if(clipped_polygon[i].x == p.first && clipped_polygon[i].y == p.second)
                    flag = true;
            }
            if(!flag) {
                intersection = Vertex(p.first, p.second, IN_OUT);
                clipped_polygon.push_back(intersection);
            }
        }
        else if(!left1 && left2) {
            bool flag = false;
            pair<int,int> p = get_intersection({polygon[i].x, polygon[i].y}, {polygon[(i+1)%polygon.size()].x, polygon[(i+1)%polygon.size()].y}, A, B);
            for(int i = 0; i < clipped_polygon.size();i++) {
                if(clipped_polygon[i].x == p.first && clipped_polygon[i].y == p.second)
                    flag = true;
            }
            if(!flag) {
                intersection = Vertex(p.first, p.second, OUT_IN);
                clipped_polygon.push_back(intersection);
            }
            // polygon[(i+1)%polygon.size()].type = INSIDE;
            clipped_polygon.push_back(polygon[(i+1)%polygon.size()]);
        }
    }
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
vector<Vertex> get_polygon_a() {
    vector<Vertex> P;
    P.push_back(Vertex(500, 500, NONE));
    P.push_back(Vertex(-500, 500, NONE));
    P.push_back(Vertex(-500, 0, NONE));
    P.push_back(Vertex(500, 0, NONE));
    return P;
}

// returns a concave polygon to be clipped
vector<Vertex> get_polygon_c() {
    vector<Vertex> P;
    P.push_back(Vertex(700, -300, NONE));
    P.push_back(Vertex(700, 300, NONE));
    P.push_back(Vertex(30, 300, NONE));
    P.push_back(Vertex(30, 200, NONE));
    P.push_back(Vertex(650, 200, NONE));
    P.push_back(Vertex(650, -200, NONE));
    P.push_back(Vertex(30, -200, NONE));
    P.push_back(Vertex(30, -300, NONE));
    return P;
}

// draws the polygon P
void draw_polygon(vector<pair<int,int>> P, float r, float g, float b) {
    glColor3f(r, g, b);
    for(int i = 0; i < P.size(); i++){ 
        draw_line(P[i], P[(i+1)%P.size()]);
    }
}

// draws the polygon P
void draw_polygon_weiler(vector<Vertex> P, float r, float g, float b) {
    glColor3f(r, g, b);
    int i = 0;
    int hook = -1;
    while(i != hook) {
        cout << P[i].x << "," << P[i].y << "," << P[i].type << endl;
        if(P[i].type == OUT_IN) {
            hook = hook == -1 ? i : hook;
            Vertex start = P[i];
            do {
                draw_line({P[i].x, P[i].y}, {P[(i+1)%P.size()].x, P[(i+1)%P.size()].y});
                i = (i+1)%P.size();
                cout << P[i].x << "," << P[i].y << "," << P[i].type << endl;
            } while(P[i].type != IN_OUT);
            draw_line({P[i].x, P[i].y}, {start.x, start.y});
        }
        i = (i+1)%P.size();
    }
    // for(int i = 0; i < P.size(); i++){ 
    //     cout << P[i].x << "," << P[i].y << "," << P[i].type << endl;
    //     draw_line({P[i].x, P[i].y}, {P[(i+1)%P.size()].x, P[(i+1)%P.size()].y});
    // }
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
