// A simple OpenGL implementation of Polygon Transformation Algorithms

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

// represents a Point object
class Point {
  public:
    int x;
    int y;
    Point(int x_, int y_) {
        this->x = x_;
        this->y = y_;
    }
};

void draw_image();
void translate_polygon(vector<Point> polygon);
void rotate_polygon(vector<Point> polygon);
void scale_polygon(vector<Point> polygon);
void sheer_polygon(vector<Point> polygon);
Point translate(Point p, int s_x, int s_y);
Point rotate(Point p, int theta);
Point scale(Point p, int s_x, int s_y);
Point sheer(Point p, int s_x, int s_y);
float convert_degree_to_rad(int angle_deg);
void reflect_1(vector<Point> polygon);
void reflect_2(vector<Point> polygon);
void reflect_3(vector<Point> polygon);
void reflect_4(vector<Point> polygon);
vector<Point> get_polygon();
void draw_polygon(vector<Point> polygon);


int main(int argc, char **argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Polygon Transformation Algorithms");
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
        vector<Point> polygon = get_polygon();
        draw_polygon(polygon);
    // glFlush();
    vector<Point> transformed_polygon;
    int command = 0;
    cout << "1. Translate the polygon with vector (a,b). \n2. Rotate the polygon with theta [0, 360).\n3. Scale the polygon with the given scaling factor.\n4. Reflect the polygon over lines x=0, y=0, x=y and x=-y.\n5. Shear the polygon with shearing factors s_x and s_y along the x and y direction.\n";
    cout << "\n Enter the command based on above: " << endl;
    cin >> command;
    glColor3f(0.0, 0.4, 1.0);
    switch (command) {
        case 1:
            translate_polygon(polygon);
            break;
        case 2:
            rotate_polygon(polygon);
            break;
        case 3:
            scale_polygon(polygon);
            break;
        case 4: { 
                reflect_1(polygon);
                reflect_2(polygon);
                reflect_3(polygon);
                reflect_4(polygon);
                break;
            }
        case 5:
            sheer_polygon(polygon);
            break;
        default:
            break;
    }
    glFlush();
}

// draws the translated version of polygon 
void translate_polygon(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    int s_x, s_y;
    cout << "Enter translation amount along x and y directions respectively: ";
    cin >> s_x >> s_y;
    for(auto p : polygon) {
        transformed_polygon.push_back(translate(p, s_x, s_y));
    }
    draw_polygon(transformed_polygon);
    glFlush();
}

// draws the rotated version of polygon 
void rotate_polygon(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    int theta;
    cout << "Enter the theta angle of rotation around the origin (0,0): ";
    cin >> theta;
    for(auto p : polygon) {
        transformed_polygon.push_back(rotate(p, theta));
    }
    draw_polygon(transformed_polygon);
}

// draws the scaled version of polygon 
void scale_polygon(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    int s_x, s_y;
    cout << "Enter scaling factor along x and y directions respectively: ";
    cin >> s_x >> s_y;
    for(auto p : polygon) {
        transformed_polygon.push_back(scale(p, s_x, s_y));
    }
    draw_polygon(transformed_polygon);
}

// draws the sheered version of polygon 
void sheer_polygon(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    int s_x, s_y;
    cout << "Enter sheering factor along x and y directions respectively: ";
    cin >> s_x >> s_y;
    for(auto p : polygon) {
        transformed_polygon.push_back(sheer(p, s_x, s_y));
    }
    draw_polygon(transformed_polygon);
}

// returns the translated version of Point p
Point translate(Point p, int s_x, int s_y) {
    return Point(p.x+s_x, p.y+s_y);
}

// returns the rotated version of Point p
Point rotate(Point p, int theta) {
    float t = convert_degree_to_rad(theta);
    int x = p.x*cos(t) - p.y*sin(t);
    int y = p.x*sin(t) + p.y*cos(t);
    return Point(x, y);
}

// returns the scaled version of Point p
Point scale(Point p, int s_x, int s_y) {
    return Point(p.x*s_x, p.y*s_y);
}

// returns the sheered version of Point p
Point sheer(Point p, int s_x, int s_y) {
    int x = p.x + s_x*p.y;
    int y = s_y*p.x + p.y;
    return Point(x, y);
}


float convert_degree_to_rad(int angle_deg) {
    return (float)angle_deg * (PI) / (float)180;
}

// reflect w.r.t. x = 0 (y-axis)
void reflect_1(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    for(auto p : polygon) {
        transformed_polygon.push_back(Point(-p.x, p.y));
    }
    draw_polygon(transformed_polygon);
}

// reflect w.r.t. y = 0 (x-axis)
void reflect_2(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    for(auto p : polygon) {
        transformed_polygon.push_back(Point(p.x, -p.y));
    }
    draw_polygon(transformed_polygon);
}


// reflect w.r.t. x = y 
void reflect_3(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    for(auto p : polygon) {
        transformed_polygon.push_back(Point(p.y, p.x));
    }
    draw_polygon(transformed_polygon);
}


// reflect w.r.t. x = -y 
void reflect_4(vector<Point> polygon) {
    vector<Point> transformed_polygon;
    for(auto p : polygon) {
        transformed_polygon.push_back(Point(-p.y, -p.x));
    }
    draw_polygon(transformed_polygon);
}


// returns a pentagon
vector<Point> get_polygon() {
    vector<Point> polygon;
    polygon.push_back({500/2, 950/2});
    polygon.push_back({850/2, 650/2});
    polygon.push_back({700/2, 250/2});
    polygon.push_back({300/2, 250/2});
    polygon.push_back({150/2, 650/2});
    return polygon;
}

// draws the polygon
void draw_polygon(vector<Point> polygon) {
    glBegin(GL_POLYGON);
    for(auto point : polygon)
        glVertex2i(point.x, point.y);
    glEnd();
}