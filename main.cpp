#include <cassert>
#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Node.hpp"
#include "Const.hpp"
#define ASSERT(pred, otherwise) assert((pred) && (otherwise))

#define radianf(x) ((float)(M_PI/180.0f*x))

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
std::function<void()> deferredDraw = [](){};
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  root->draw();
  deferredDraw();
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
          Node::create("test", Transform(), std::make_shared<SweptSurface>(SweptSurface::create(SweptSurfaceData::load("data/model.txt"))))
      }
  );
  last_time_point = steady_clock::now();
}

Eigen::Vector2i windowSize = Eigen::Vector2i(500, 500);
float fov = 70.0f;
float distance = 5.0;
Eigen::Vector3f viewCenter = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
Eigen::Vector3f camDir = -Eigen::Vector3f(1.0, 1.0, 1.0f).normalized();
Eigen::Vector3f camUp = (up - camDir * camDir.dot(up)).normalized();

void setView() {
  glViewport(0, 0, static_cast<GLsizei>(windowSize[0]), static_cast<GLsizei>(windowSize[1]));

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  auto distancedEye = viewCenter - camDir * distance;
  gluPerspective(fov, static_cast<GLdouble>(windowSize[0]) / static_cast<GLdouble>(windowSize[1]), .1f, 500.0f);
  gluLookAt(distancedEye[0], distancedEye[1], distancedEye[2],
            viewCenter[0], viewCenter[1], viewCenter[2],
            camUp[0], camUp[1], camUp[2]);
  glutPostRedisplay();
}

void reshape(int w, int h) {
  windowSize = Eigen::Vector2i(w, h);
  setView();
}

const float virtualTrackballRadius = 2.0f;
bool leftButton = false;
bool rightButton = false;
Eigen::Vector2f mousePos;
Eigen::Vector3f initialCenter;
Eigen::Vector3f initialDir;
Eigen::Vector3f initialUp;

Eigen::Vector2f normalizeXY(int x, int y) {
  return (Eigen::Vector2i(x, y) - windowSize/2).cast<float>() / std::min(windowSize[0], windowSize[1]);
}

Eigen::Quaternionf toQuaternion(Eigen::Vector3f x) {
  auto a = cosf(x.norm());
  Eigen::Vector3f v = x.normalized() * sinf(x.norm());
  return Eigen::Quaternionf(a, v[0], v[1], v[2]);
}

Eigen::Vector3f getVirtualTrackballXYZ(const Eigen::Vector2f a, const Eigen::Vector3f right, const Eigen::Vector3f up, const Eigen::Vector3f front) {
  const auto xy = (a.norm() > 1)? a.normalized() : a;
  const auto Z = sqrtf(std::max(0.0f, 1.0f-xy.squaredNorm()));
  return (xy.x() * right + xy.y() * up + Z * front).normalized();
}

void motion(int x, int y) {
  const auto curr = normalizeXY(x, y);
  const auto projDist = 0.5f / tanf(radianf(fov)/2);

  const auto u_dir = initialDir;
  const auto u_front = -initialDir;
  const auto u_up = initialUp;
  const auto u_right = initialUp.cross(initialDir).normalized();

  if (leftButton) {
    const auto diff = curr - mousePos;

    viewCenter = initialCenter + (u_right * diff[0] + u_up * diff[1]) * distance/projDist;
  }
  else if (rightButton) {
    const auto from = getVirtualTrackballXYZ(mousePos * 2, u_right, u_up, u_front);
    const auto to = getVirtualTrackballXYZ(curr * 2, u_right, u_up, u_front);

    const auto rot = Eigen::Quaternionf::FromTwoVectors(from, to);

    camDir = (rot * toQuaternion(initialDir) * rot.inverse()).vec().normalized();
    camUp = (rot * toQuaternion(initialUp) * rot.inverse()).vec().normalized();
  }
  setView();
}

void mouse(int button, int state, int x, int y) {
  switch ( button )
  {
  case GLUT_LEFT_BUTTON:
    if (rightButton) break;
    if ( state == GLUT_DOWN )
    {
      mousePos = normalizeXY(x, y);
      initialCenter = viewCenter;
      initialDir = camDir;
      initialUp = camUp;
      leftButton = true;
    }
    else if ( state == GLUT_UP )
    {
      leftButton = false;
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (leftButton) break;
    if ( state == GLUT_DOWN )
    {
      mousePos = normalizeXY(x, y);
      initialDir = camDir;
      initialUp = camUp;
      rightButton = true;
    }
    else if ( state == GLUT_UP )
    {
      rightButton = false;
    }
    break;
  case 3:
    if ( state == GLUT_DOWN )
    {
      if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
        fov = std::max(fov/1.1f, 10.0f);
      } else {
        distance /= 1.1;
      }
    }
    break;
  case 4:
    if ( state == GLUT_DOWN )
    {
      if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
        fov = std::min(fov * 1.1f, 179.0f);
      } else {
        distance *= 1.1;
      }
    }
    break;
  default:break;
  }
  setView();
  return;
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
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutMainLoop();
  glutDestroyWindow(window);

  return 0;
}
