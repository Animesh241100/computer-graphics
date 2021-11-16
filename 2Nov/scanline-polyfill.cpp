// A simple OpenGL implementation of Scanline Polyfill Algorithm

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
#define SIDE 800
#define MAX 1000

using namespace std;

// represents a single cell in the SET bucket
class Cell {
  public:
    int y_max;
    int x_min;
    int dx;
    int dy;
    int sign;
    int count;
    Cell(int y_max_, int x_min_, int dx_, int dy_) {
        this->x_min = x_min_;
        this->y_max = y_max_;
        this->dx = abs(dx_);
        this->dy = abs(dy_);
        this->sign = (dx_*dy_ < 0) ? -1 : 1;
        this->count = 0;
    }
};

// Represnts an edge of a polygon
class Edge {
  public:
    pair<int,int> A;
    pair<int,int> B;
    Edge(pair<int,int> A_, pair<int,int> B_) {
        this->A = A_;
        this->B = B_;
    }
};

void draw_image();
void scanline_poly_fill(vector<Edge> boundary);
void update_x_values(vector<Cell> &AET, int y);
void remove_dead_entries(vector<Cell> &AET, int y);
void draw_scan_line(vector<Cell> &AET, int y);
void merge_bucket(map<int, vector<Cell>> &SET, int y, vector<Cell> &AET);
bool AETComparator(const Cell C1,  const Cell C2);
map<int, vector<Cell>> get_sorted_edge_table(vector<Edge> boundary);
Cell get_set_cell(Edge &edge);
int get_y_min(map<int, vector<Cell>> &SET);
vector<Edge> get_boundary();
void draw_polygon(vector<Edge> P, float r, float g, float b);
void get_updated_x(Cell &cell);
void print_set(map<int, vector<Cell>> SET);
void print_row(vector<Cell> &AET);

void plot_point(int x1, int y1);
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair_bld(pair<float,float> &curr_pair, float &temp_dp, float val1, float val2);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scanline Polyfill Algorithm");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}

// draw the overall image
void draw_image() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glOrtho(-MAX, MAX, -MAX, MAX, -MAX, MAX);
        vector<Edge> boundary = get_boundary();
        draw_polygon(boundary, 0.0, 0.0, 1.0);
        glFlush();
        scanline_poly_fill(boundary);
    glFlush();
}

// iterates the scanline polyfill region filling algorithm inside the polygon boundary 'boundary'
void scanline_poly_fill(vector<Edge> boundary) {
    map<int, vector<Cell>> SET = get_sorted_edge_table(boundary);
    int y = get_y_min(SET);
    vector<Cell> AET;
    do {
        merge_bucket(SET, y, AET);
        sort(AET.begin(), AET.end(), AETComparator);
        draw_scan_line(AET, y);
        y++;
        remove_dead_entries(AET, y);
        update_x_values(AET, y);
        usleep(5000);
    } while(!AET.empty() || (SET.find(y) != SET.end()));
}

// update all the Cells in the AET with respect to the new scanline
void update_x_values(vector<Cell> &AET, int y) {
    for(auto &cell : AET) {
        if(cell.dx != 0) { // non vertical edge
            get_updated_x(cell);
        }
    }
}

// update the x_min value with respect to the integer arithmetic based algorithm
void get_updated_x(Cell &cell) {
    int d_counter = min(cell.dx, cell.dy);
    cell.count += d_counter;
    if(cell.count >= cell.dy) {
        cell.count -= cell.dy;
        cell.x_min += cell.sign;
    }
}

// remove the entries for which the y_max is the same as the present scanline y
void remove_dead_entries(vector<Cell> &AET, int y) {
    while(1) {
        int i = 0;
        for(i = 0; i < AET.size(); i++) {
            if(AET[i].y_max == y) {
                AET.erase(AET.begin() + i);
                break;
            }
        }
        if(i == AET.size())
            break;
    }
}

// draw the scanline 'y' with respect to the entries in the AET
void draw_scan_line(vector<Cell> &AET, int y) {
    if(AET.size() != 2)
        return;
    for(int i = AET[0].x_min; i <= AET[1].x_min; i++)
        plot_point(i, y);
}

// merge the SET buckets for the scanline 'y' into the active edge table AET
void merge_bucket(map<int, vector<Cell>> &SET, int y, vector<Cell> &AET) {
    auto it = SET.find(y);
    if(it == SET.end())
        return;
    AET.insert(AET.end(), it->second.begin(), it->second.end());
}

// A comparator to sort the active edge table AET
bool AETComparator(const Cell C1,  const Cell C2) {
    return (C1.x_min < C2.x_min);
}

// returns the sorted edge table with respect to the give polygon 'boundary'
map<int, vector<Cell>> get_sorted_edge_table(vector<Edge> boundary) {
    map<int, vector<Cell>> SET;
    for(auto edge : boundary) {
        int y_min = min(edge.A.second, edge.B.second);
        Cell cell = get_set_cell(edge);
        if(cell.dy == 0)
            continue;
        auto it = SET.find(y_min);
        if(it != SET.end()) 
            it->second.push_back(cell);
        else {
            vector<Cell> bucket = {cell};
            SET.insert({y_min, {cell}});
        }
    }
    return SET;
}

// returns the SET cell with respect to the given 'edge' object
Cell get_set_cell(Edge &edge) {
    int y_max = max(edge.A.second, edge.B.second);
    int x_min = edge.A.second < edge.B.second ? edge.A.first : edge.B.first;
    int dx = edge.A.first - edge.B.first;
    int dy = edge.A.second - edge.B.second;
    return Cell(y_max, x_min, dx, dy);
}

// returns the minimum y value(the lowest scanline) in the sorted edge table SET
int get_y_min(map<int, vector<Cell>> &SET) {
    int y_min = (int)INFINITY;
    for(auto it : SET) {
        if(it.first < y_min)
            y_min = it.first;
    }
    return y_min;
}

// returns a window polygon
vector<Edge> get_boundary() {
    vector<Edge> B;
    B.push_back(Edge({SIDE,0}, {SIDE-round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))}));
    B.push_back(Edge({SIDE-round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))}, {-SIDE+round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))}));
    B.push_back(Edge({-SIDE+round(SIDE*cos(PI/3)),round(SIDE*sin(PI/3))}, {-SIDE,0}));
    B.push_back(Edge({-SIDE,0}, {-SIDE+round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))}));
    B.push_back(Edge({-SIDE+round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))}, {SIDE-round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))}));
    B.push_back(Edge({SIDE-round(SIDE*cos(PI/3)),(-1)*round(SIDE*sin(PI/3))}, {SIDE,0}));
    return B;
}

// draws the polygon P
void draw_polygon(vector<Edge> P, float r, float g, float b) {
    glColor3f(r, g, b);
    for(int i = 0; i < P.size(); i++){ 
        draw_line(P[i].A, P[i].B);
    }
}

// prints the SET table
void print_set(map<int, vector<Cell>> SET) {
    for(auto it : SET) {
        cout << it.first << ": ---[";
        print_row(it.second);
    }
    cout << "\n\n";
}

// prints the AET table
void print_row(vector<Cell> &AET){
    for(auto cell : AET) {
        cout << "(" << cell.y_max << "," << cell.x_min << "," << cell.dx << "," << cell.dy << "," << cell.sign << "), ";
    }
    cout << endl;
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
    glEnd();
}