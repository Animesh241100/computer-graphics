/*

Roll Number: CED18I065
Name: ANIMESH KUMAR

ICG Lab Mid Term Test

Question: Using OpenGL, draw the first 4 letters of your name in CAPITAL letters using 
General two-point formula line drawing algorithm and Mid-point Circle/Circular Arc drawing 
algorithm. The dimensions of the various components in  the figures can be considered as you 
wish, but the overall shape should  be  the same as asked in the question. 


Therefore the shape to be made is ANIM
*/

#include "GL/freeglut.h"
#include "GL/gl.h"
#include<math.h>
#include<utility>
#include<iostream>

using namespace std;


void draw_image();
void draw_A();
void draw_N();
void draw_I();
void draw_M();
void draw_general_line(pair<int,int> A, pair<int,int> B);
void handle_case_1(pair<int,int>A, pair<int,int>B, float slope);
void handle_case_2(pair<int,int>A, pair<int,int>B, float slope);
int get_next_y_coordinate(int x, float slope, pair<int,int> A);
int get_next_x_coordinate(int y, float slope, pair<int,int> A);


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ICG Lab EndSem");
    glutDisplayFunc(draw_image);
    glutMainLoop();
    return 0;
}



void draw_image() {
    glClearColor(1.0, 1.0, 1.0, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glOrtho(-1000, 1000, -1000, 1000, -1000, 1000);
        draw_A();
        draw_N();
        draw_I();
        draw_M();
    glFlush();
}

// Draws A
void draw_A() {
    glPointSize(3);
    glBegin(GL_POINTS);
    draw_general_line({-950, -250}, {-690, 250});
    draw_general_line({-690, 250}, {-660, 250});
    draw_general_line({-660, 250}, {-425, -250});
    draw_general_line({-835, -60}, {-527, -60});
    glEnd();
}

// Draws N
void draw_N() {
    glPointSize(3);
    glBegin(GL_POINTS);
    draw_general_line({-400, -250}, {-400, 250});
    draw_general_line({-10, -250}, {-10, 250});
    draw_general_line({-400, 250}, {-10, -250});
    glEnd();
}

// Draws I
void draw_I() {
    glPointSize(3);
    glBegin(GL_POINTS);
    draw_general_line({40, 250}, {420, 250});
    draw_general_line({225, 250}, {225, -250});
    draw_general_line({40, -250}, {420, -250});
    glEnd();
}

// Draws M
void draw_M() {
    glPointSize(3);
    glBegin(GL_POINTS);
    draw_general_line({500, 250}, {500, -250});
    draw_general_line({500, 250}, {725, 0});
    draw_general_line({725, 0}, {950, 250});
    draw_general_line({950, 250}, {950, -250});
    glEnd();
}


/*********************** Utility Functions to Draw Straight Line ************************/

// draws a straight line from the Point A to Point B using the General Line drawing algorithm
void draw_general_line(pair<int,int> A, pair<int,int> B) {
    float slope = (float)(B.second - A.second) / (float)(B.first - A.first);
    if(abs(slope) < 1)
        handle_case_1(A, B, slope);
    else 
        handle_case_2(A, B, slope);
}

// handles the case when slope's absolute value < 1
void handle_case_1(pair<int,int>A, pair<int,int>B, float slope) {
    if(A.first > B.first)
        A.swap(B);  // to make the x coordinate of A lesser than B
    int x_i, y_i;
    x_i = A.first;
    while(x_i <= B.first) {
        y_i = get_next_y_coordinate(x_i, slope, A);
        glVertex2i(x_i, y_i);
        x_i++;
    }
}

// handles the case when slope's absolute value >= 1
void handle_case_2(pair<int,int>A, pair<int,int>B, float slope) {
    if(A.second > B.second)
        A.swap(B); // to make the y coordinate of A lesser than B
    int x_i, y_i;
    y_i = A.second;
    while(y_i <= B.second) {
        x_i = get_next_x_coordinate(y_i, slope, A);
        glVertex2i(x_i, y_i);
        y_i++;
    }
}


int get_next_y_coordinate(int x, float slope, pair<int,int> A) {
    return round((x-A.first) * slope + A.second);
}

int get_next_x_coordinate(int y, float slope, pair<int,int> A) {
    return round((float)(y - A.second) / slope + A.first);
}

/**********************************************************************************************/
