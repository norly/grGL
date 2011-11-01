#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "graph.h"

/****************************
 * Graph memory
 ***************************/

// Free the memory for a graph and substructures
void grFree(grGraph *g)
{
  if (g)
  {
    if (g->nodes)
      free(g->nodes);
    if (g->edges)
      free(g->edges);
    free(g);
  }
}


// Allocate memory for a new graph, return NULL on any error
grGraph* grAlloc(int numnodes, int numedges)
{
  grGraph *g;

  g = malloc(sizeof(grGraph));
  if (!g)
    return NULL;

  g->nodes = malloc(sizeof(grNode[numnodes]));
  g->edges = malloc(sizeof(grEdge[numedges]));
  if (!g->nodes || !g->edges)
    goto ERR;

  g->numnodes = numnodes;
  g->numedges = numedges;

  return g;

  ERR:
  grFree(g);
  return NULL;
}



/****************************
 * Graph generation
 ***************************/

// Randomize a graph
void grGenRandom(grGraph *g)
{
  int i;
  int ns = g->numnodes;
  int es = g->numedges;

  for (i = 0; i < ns; i++)
  {
    grNode *n = &g->nodes[i];

    n->x = (float)rand() / (RAND_MAX/4);
    n->y = (float)rand() / (RAND_MAX/4);
    n->z = (float)rand() / (RAND_MAX/4);
    n->colour = 0.0;
  }

  for (i = 0; i < es; i++)
  {
    grEdge *e = &g->edges[i];

    e->n1 = rand() / (RAND_MAX/ns);
    e->n2 = rand() / (RAND_MAX/ns);
  }
}



/****************************
 * Graph drawing functions
 ***************************/

// Draw the graph on the current OpenGL context
void graphDrawGL(grGraph *g)
{
  int i;
  int ns = g->numnodes;
  int es = g->numedges;

  // Draw edges
  glColor4f(0.0, 0.0, 1.0, 0.5);
  for (i = 0; i < es; i++)
  {
    grNode *n1 = &g->nodes[g->edges[i].n1];
    grNode *n2 = &g->nodes[g->edges[i].n2];

    glBegin(GL_LINE_STRIP);
      glVertex3f(n1->x, n1->y, n1->z);
      glVertex3f(n2->x, n2->y, n2->z);
    glEnd();
  }

  // Draw nodes
  glPointSize(5.0);
  glBegin(GL_POINTS);
    for (i = 0; i < ns; i++)
    {
      grNode *n = &g->nodes[i];
      //glLoadName(i);    // Load point number into depth buffer for selection
      glColor4f(n->colour, 1.0, 0.3, 0.7);
      glVertex3f(n->x, n->y, n->z);
    }
  glEnd();
}
