#include <cassert>
#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Node.hpp"
#include "Drawables.hpp"
#include "Const.hpp"

void display();
void init();
void update();
void reshape(int w, int h);

std::shared_ptr<Node> root;

void drawAxis(GLfloat size = 1.0f);
void drawAxis(GLfloat size) {
    glPushMatrix();

    glPopMatrix();
}

void display()
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    root->draw();
    glFlush ();
    glutSwapBuffers();
}

const int WINDOW_WIDTH = 250;
const int WINDOW_HEIGHT = 250;

void init ()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

    glShadeModel (GL_SMOOTH);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    GLfloat light_position[] = { 1.0f, 1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat light_direction[] = { -1.0f, -1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    root = std::make_shared<Node>("root", Transform());
    root->insert({
        std::make_shared<Node>("child", Transform(), std::make_shared<Axis>()),
        std::make_shared<Node>("child", Transform(), std::make_shared<Teapot>())
    });
}

void reshape (int w, int h)
{
    glViewport (0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0f, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), .1f, 500.0f);
    gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
}

void update()
{

}

#define ASSERT(pred, otherwise) assert((pred) && (otherwise))

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    auto screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    auto screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    ASSERT(screenWidth > 0, "Failed to glutGet(GLUT_SCREEN_WIDTH)");
    ASSERT(screenHeight > 0, "Failed to glutGet(GLUT_SCREEN_WIDTH)");

    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

    glutInitWindowPosition ((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2);

    auto window = glutCreateWindow ("snu-graphics-18: HW 1");
    init ();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutMainLoop();
    glutDestroyWindow(window);
    return 0;
}
