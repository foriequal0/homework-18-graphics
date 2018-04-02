#include <cassert>
#include <vector>
#include <chrono>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Node.hpp"
#define ASSERT(pred, otherwise) assert((pred) && (otherwise))

void display();
void init();
void update();
void reshape(int w, int h);

std::shared_ptr<Node> root;

int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 500;

using steady_clock = std::chrono::steady_clock;
using duration = std::chrono::duration<double, std::chrono::seconds::period>;
using time_point = std::chrono::time_point<steady_clock, duration>;

const duration T = std::chrono::duration_cast<duration>(std::chrono::seconds(2));
time_point last_time_point;

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  root->draw();
  glFlush();
  glutSwapBuffers();
}

void init() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat light_position[] = {1.0f, 1.0f, -1.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  GLfloat light_direction[] = {-1.0f, -1.0f, 1.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
  GLfloat light_ambient[] = {0.0f, 0.2f, 0.8f, 1.0f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

  root = Node::create("root", Transform());
  root->insert(
      {
          Node::create("UB", Transform().setPosition(0, 1, 0), nullptr, std::vector<std::shared_ptr<Node>>
              {
                Node::create("UB_shape", Transform().setScale(1, 1, 0.3), std::make_shared<Cube>()),
                Node::create("H", Transform().setPosition(0, 1, 0), nullptr, std::vector<std::shared_ptr<Node>>
                    {
                      Node::create("H_shape", Transform().setScale(0.5, 0.5, 0.5).setOrientation(0, -90, 0), std::make_shared<Teapot>())
                    }),
                Node::create("LUA", Transform().setPosition((1.0f + 0.1f)/2.0f, 1.0f/2.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                    {
                        Node::create("LUA_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.1, 1, 0.1), std::make_shared<Cube>()),
                        Node::create("LLA", Transform().setPosition(0, -1.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                            {
                                Node::create("LLA_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.1, 1, 0.1), std::make_shared<Cube>())
                            })
                    }),
                Node::create("RUA", Transform().setPosition(-(1.0f + 0.1f)/2.0f, 1.0f/2.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                    {
                        Node::create("RUA_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.1, 1, 0.1), std::make_shared<Cube>()),
                        Node::create("RLA", Transform().setPosition(0, -1.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                            {
                                Node::create("RLA_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.1, 1, 0.1), std::make_shared<Cube>())
                            })
                    }),
          }),
          Node::create("LB", Transform(), nullptr, std::vector<std::shared_ptr<Node>>{
              Node::create("LB_shape", Transform().setScale(1, 1, 0.3), std::make_shared<Cube>()),
              Node::create("LUL", Transform().setPosition((1.0f - 0.4f)/2.0f, -1.0f/2.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                  {
                      Node::create("LUL_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.3, 1, 0.3), std::make_shared<Cube>()),
                      Node::create("LLL", Transform().setPosition(0, -1.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                          {
                              Node::create("LLA_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.3, 1, 0.3), std::make_shared<Cube>()),
                              Node::create("LF", Transform().setPosition(0, -1, 0.25).setScale(0.3, 0.2, 0.5), std::make_shared<Cube>())
                          })
                  }),
              Node::create("RUL", Transform().setPosition(-(1.0f - 0.4f)/2.0f, -1.0f/2.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                  {
                      Node::create("RUL_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.3, 1, 0.3), std::make_shared<Cube>()),
                      Node::create("RLL", Transform().setPosition(0, -1.0f, 0), nullptr, std::vector<std::shared_ptr<Node>>
                          {
                              Node::create("RLA_shape", Transform().setPosition(0, -0.5f, 0).setScale(0.3, 1, 0.3), std::make_shared<Cube>()),
                              Node::create("RF", Transform().setPosition(0, -1, 0.25).setScale(0.3, 0.2, 0.5), std::make_shared<Cube>())
                          })
                  }),
          })
      });
  last_time_point = steady_clock::now();
}

void reshape(int w, int h) {
  WINDOW_WIDTH = w;
  WINDOW_HEIGHT = h;
  glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0f, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), .1f, 500.0f);
  gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);
}


void update() {
  auto now = steady_clock::now();
  auto t = now - last_time_point;
  if (t > T) {
    auto times = floor(t / T);
    t -= times * T;
    last_time_point = now - t;
  }
  double r = t/T;
  float s = sinf(r * 2 * M_PI);
  s = (s>0)?powf(s, 0.75):-powf(-s, 0.75);
  float c = cosf(r * 2 * M_PI);

  root->transform.setPosition(0, abs(sinf(r * 2 * M_PI)) * 0.3, 0);
  auto ub = root->get("UB");
  ub->transform.setOrientation(10 - sinf(r*2 * 2 * M_PI) * 5, s * 20, 0);
  ub->get("H")->transform.setOrientation(0, -s * 15, 0);

  ub->get("LUA")->transform.setOrientation(s * 45, (s+1) * 5, 0);
  ub->get("RUA")->transform.setOrientation(- s * 45, (-s+1) * 5, 0);

  ub->get("LUA")->get("LLA")->transform.setOrientation(-135+(s+0.5) * 10, 0, 0);
  ub->get("RUA")->get("RLA")->transform.setOrientation(-135+(-s+0.5) * 10, 0, 0);

  auto lb = root->get("LB");
  lb->transform.setOrientation(cosf(r*2 * 2 * M_PI) * 5, -s * 20, 0);
  lb->get("LUL")->transform.setOrientation(-s * 60 - 30, (-s) * 5, (-s) * 5);
  lb->get("RUL")->transform.setOrientation(s * 60 - 30, (s) * 5, (s) * 5);

  lb->get("LUL")->get("LLL")->transform.setOrientation(s * 60 + 60, 0, 0);
  lb->get("RUL")->get("RLL")->transform.setOrientation(-s * 60 + 60, 0, 0);
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  auto screenWidth = glutGet(GLUT_SCREEN_WIDTH);
  auto screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
  ASSERT(screenWidth > 0, "Failed to glutGet(GLUT_SCREEN_WIDTH)");
  ASSERT(screenHeight > 0, "Failed to glutGet(GLUT_SCREEN_WIDTH)");

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutInitWindowPosition((screenWidth - WINDOW_WIDTH) / 2, (screenHeight - WINDOW_HEIGHT) / 2);

  auto window = glutCreateWindow("snu-graphics-18: HW 1");
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(update);
  glutMainLoop();
  glutDestroyWindow(window);
  return 0;
}
