#include <GL/gl.h>
#include <GL/glu.h>

#include "glancillary.h"



/****************************
 * GL init
 ***************************/

void glaInit(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  //glShadeModel(GL_SMOOTH);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Init Model/view transformation
  glMatrixMode(GL_MODELVIEW);

  // Reset MV matrix
  glLoadIdentity();
}




/****************************
 * GL camera simulation helpers
 ***************************/

void glaCameraRotatef(GLfloat *cmat, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  glPushMatrix();
    glLoadIdentity();
    glRotatef(angle, x, y, z);
    glMultMatrixf(cmat);
    glGetFloatv(GL_MODELVIEW_MATRIX, cmat);
  glPopMatrix();
}

void glaCameraTranslatef(GLfloat *cmat, GLfloat x, GLfloat y, GLfloat z)
{
  glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, z);
    glMultMatrixf(cmat);
    glGetFloatv(GL_MODELVIEW_MATRIX, cmat);
  glPopMatrix();
}




/****************************
 * GL box drawing example
 ***************************/

void glaDrawExample(void)
{
  // Draw the corner dots
  glPointSize(5.0);
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_POINTS);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
  glEnd();

  // Draw a line with fixed vertices
  glColor3f(0.5, 1.0, 0.5);
  glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.5);
  glEnd();

  // Draw the box boundaries
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINE_STRIP);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
  glEnd();
}










/****************************
 * GL selection (currently defunct)
 ***************************/

// Selection helper, currently defunct.
void glaSelect(int x, int y)
{
  int i;
  GLuint namebuf[64] = {0};
  GLint hits, view[4];
  GLfloat projmat[16];

  // Account for inverse Y coordinate
  glGetIntegerv(GL_VIEWPORT, view);
  y = view[3] - y;

  glSelectBuffer(64, namebuf);
  glRenderMode(GL_SELECT);

  // Reset name stack
  glInitNames();
  glPushName(0);

  // Restrict projection matrix to selection area
  glGetFloatv(GL_PROJECTION_MATRIX, projmat);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(x, y, 1.0, 1.0, view);
    glMultMatrixf(projmat);

    // Redraw points to fill selection buffer
    glMatrixMode(GL_MODELVIEW);
    //glutSwapBuffers();
    //drawPoints();

    // Reset projection
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  hits = glRenderMode(GL_RENDER);

  for (i = 0; i < hits; i++)
    // Do something with the namebuf element here
    break;

  glMatrixMode(GL_MODELVIEW);
}
