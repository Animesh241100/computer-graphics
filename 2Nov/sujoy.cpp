#include<bits/stdc++.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<unistd.h>
using namespace std;

int left_edge[500], right_edge[500];
int x1 = 100;
int y11 = 250;
int x3 = 400;
int y3 = 250;
int x4 = 250;
int y4 = 400;
int x2 = 250;
int y2 = 100;


void myDisplay();
void scanline_Filling(int x1, int y11, int x2, int y2, int x3, int y3, int x4, int y4);
void edge_Detect(int x1, int y11, int x2, int y2);
void draw_pixel(int x, int y);


void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    scanline_Filling(x1, y11, x2, y2, x3, y3, x4, y4);
    glFlush();
}

void scanline_Filling(int x1, int y11, int x2, int y2, int x3, int y3, int x4, int y4)
{
    for (int i = 0; i < 500; i++)
    {
        left_edge[i] = 500;     // Intial value
        right_edge[i] = 0;
    }
    
    // Test edge for individual lines
    edge_Detect(x1, y11, x2, y2);
    edge_Detect(x2, y2, x3, y3);
    edge_Detect(x3, y3, x4, y4);
    edge_Detect(x4, y4, x1, y11);
    
    
    // Printing polygon
    for (int y = 0; y < 500; y++)
    {
        if (left_edge[y] <= right_edge[y])
        {
            for (int x = left_edge[y]; x < right_edge[y]; x++)
            {
                draw_pixel(x, y);
            }
        }
        usleep(9000);
    }
    
}

void edge_Detect(int x1, int y11, int x2, int y2)
{
    // Slope shouldnt be in Negative
    if (y2 < y11)  {
        swap(x1, x2);
        swap(y11, y2);
    }
    
    // Calculate Increment of X if line is parallel or.. find reciprocol of slope
    double increment_x = (y2 == y11)? (x2 - x1) : (x2 - x1) / (y2 - y11);
    
    double x = x1;  // Initial value of x
    for (int y = y11; y < y2; y++)   // while y stays in the line
    {
        left_edge[y] = (x < left_edge[y])? x : left_edge[y];
        right_edge[y] = (x > right_edge[y])? x : right_edge[y];
        x += increment_x;
    }
}

void draw_pixel(int x, int y)   // Plot a point at (x, y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}


void myInit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Scanline Polyfill");
    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();
    return 0;
}