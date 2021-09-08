// A simple OpenGL program that draws a polygon.

#include "GL/freeglut.h"
#include "GL/gl.h"

void drawTriangle()
{
    glClearColor(1.0, 0.4, 0.4, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        glBegin(GL_POLYGON);
                glVertex3f(0, 0.5, 0);
                glVertex3f(0.5, 0.2, 0);
                glVertex3f(0.5, 0, 0);
                glVertex3f(0.5, -0.2, 0);
                glVertex3f(0, -0.5, 0);
                glVertex3f(-0.5, -0.2, 0);
                glVertex3f(-0.5, 0, 0);
                glVertex3f(-0.5, 0.2, 0);
        glEnd();

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a triangle");
    glutDisplayFunc(drawTriangle);
    glutMainLoop();
    return 0;
}