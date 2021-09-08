// A simple OpenGL program that draws a hexagon.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<time.h>
#include<utility>

#define PI 3.14159     // Mathematical Constant PI

using namespace std;


void draw_hexagon();
void draw_line(pair<float,float> src, pair<float,float> dest);
void get_next_pair(pair<float,float> &curr_pair, float m, bool decrement) ;



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
    glFlush();
}


//draws a line from `src` co-ordinate to `dest` co-ordinate
void draw_line(pair<float,float> src, pair<float,float> dest) {
    float m = (float)(dest.second - src.second) / (float)(dest.first - src.first);
    bool decrement = false;
    if(abs(m) >= 1)
        decrement = dest.second >= src.second ? false : true;
    else
        decrement = dest.first >= src.first ? false : true;
    glPointSize(3);
    pair<float,float> temp = src;
    while(1) {
        if(round(temp.first) == round(dest.first) && round(temp.second) == round(dest.second))
            break;
        glBegin(GL_POINTS);
            glVertex2i(round(temp.first), round(temp.second));
        glEnd();
        get_next_pair(temp, m, decrement);
    }
}


// modifies the curr_pair to the next co-ordinate pair value
void get_next_pair(pair<float,float> &curr_pair, float m, bool decrement) {
    int inc = 1;
    if(decrement) {
        inc = (-1)*inc;
        m = (-1)*m;
    }
    if(abs(m) >= 1) {
        curr_pair.second = curr_pair.second + inc;
        curr_pair.first = curr_pair.first + (float)1 / m;
    } else {
        curr_pair.first = curr_pair.first + inc;
        curr_pair.second = curr_pair.second + m; 
    }
}