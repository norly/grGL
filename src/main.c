// A simple OpenGL graph renderer.
// -- norly.

#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "graph.h"
#include "glancillary.h"


enum MOUSE_MODES {
  MOUSE_IDLE,
  MOUSE_ROTATING,
  MOUSE_TRANSLATING,
  MOUSE_TRANSLATING2,
  MOUSE_DRAGGING
};

enum MOUSE_MODES mouse_mode = MOUSE_IDLE;
int mouse_x, mouse_y;
GLfloat cameramat[16];

// The graph itself.
grGraph *g = NULL;




/****************************
 * GLUT window event handlers
 ***************************/

void on_display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the camera
  glLoadMatrixf(cameramat);

  // Draw the old example objects
  glaDrawExample();

  // Draw the graph
  graphDrawGL(g);

  glFlush();
  glutSwapBuffers();
}

void on_reshape(int w, int h)
{
  // Set up the Viewport transformation
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  // Set up the Projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (GLfloat)w/(GLfloat)h, 0.0001, 1000.0);

  // Switch to Model/view transformation for drawing objects
  glMatrixMode(GL_MODELVIEW);
}




/****************************
 * GLUT input event handlers
 ***************************/

void on_keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
    case 'Q':
    case 'q':
      exit(0);
      break;
    case 'a':
      glaCameraTranslatef(cameramat, 1.0, 0.0, 0.0);
      break;
    case 'd':
      glaCameraTranslatef(cameramat, -1.0, 0.0, 0.0);
      break;
    case 'w':
      glaCameraTranslatef(cameramat, 0.0, 0.0, 1.0);
      break;
    case 's':
      glaCameraTranslatef(cameramat, 0.0, 0.0, -1.0);
      break;
    case '=':
      //glScalef(1.5, 1.5, 1.5);
      break;
    case '-':
      //glScalef(1/1.5, 1/1.5, 1/1.5);
      break;
  }
  glutPostRedisplay();
}

void on_specialKeyboard(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_LEFT:
      glaCameraRotatef(cameramat, -15.0, 0.0, 1.0, 0.0);
      break;
    case GLUT_KEY_UP:
      glaCameraTranslatef(cameramat, 0.0, -1.0, 0.0);
      break;
    case GLUT_KEY_RIGHT:
      glaCameraRotatef(cameramat, 15.0, 0.0, 1.0, 0.0);
      break;
    case GLUT_KEY_DOWN:
      glaCameraTranslatef(cameramat, 0.0, 1.0, 0.0);
      break;
  }
  glutPostRedisplay();
}



void on_mousebutton(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
  {
    if (mouse_mode != MOUSE_IDLE)
      return;

    switch(glutGetModifiers())
    {
      case 0:   // When no modifiers are pressed
        //mouse_mode = MOUSE_DRAGGING;
        //glaSelect(x, y);
        //break;
      case GLUT_ACTIVE_SHIFT:
        mouse_mode = MOUSE_ROTATING;
        break;
      case GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL:
        mouse_mode = MOUSE_TRANSLATING;
        break;
      case GLUT_ACTIVE_CTRL:
        mouse_mode = MOUSE_TRANSLATING2;
        break;
    }

    mouse_x = x;
    mouse_y = y;
  }
  else if (state == GLUT_UP)
    mouse_mode = MOUSE_IDLE;
}

void on_mousemove(int x, int y)
{
  int dx, dy;

  if (mouse_mode != MOUSE_IDLE)
  {
    dx = x - mouse_x;
    dy = y - mouse_y;
    mouse_x = x;
    mouse_y = y;

    glPushMatrix();
      glLoadIdentity();
      switch(mouse_mode)
      {
        case MOUSE_TRANSLATING:
          glTranslatef((0.1) * dx, (-0.1) * dy, 0.0);
          break;
        case MOUSE_TRANSLATING2:
          glRotatef(dx, 0.0, 1.0, 0.0);
          glTranslatef(0.0, 0.0, (-0.1) * dy);
          break;
        case MOUSE_ROTATING:
          glRotatef(dx, 0.0, 1.0, 0.0);
          glRotatef(dy, 1.0, 0.0, 0.0);
          break;
        case MOUSE_DRAGGING:
        case MOUSE_IDLE:
          break;
      }
      glMultMatrixf(cameramat);
      glGetFloatv(GL_MODELVIEW_MATRIX, cameramat);
    glPopMatrix();
  }
  glutPostRedisplay();
}




/****************************
 * Main function
 ***************************/

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);

  // Init graph and GL
  g = grAlloc(1000, 6000);
  srand(0);
  grGenRandom(g);
  glaInit ();

  // Init camera matrix
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glGetFloatv(GL_MODELVIEW_MATRIX, cameramat);
  glLoadIdentity();

  // Register callbacks
  glutDisplayFunc(on_display);
  glutReshapeFunc(on_reshape);
  glutKeyboardFunc(on_keyboard);
  glutSpecialFunc(on_specialKeyboard);
  glutMouseFunc(on_mousebutton);
  glutMotionFunc(on_mousemove);

  glutMainLoop();

  grFree(g);
  return 0;
}
