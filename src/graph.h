#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct {
  int n1;
  int n2;
} grEdge;


typedef struct {
  float x;
  float y;
  float z;
  float colour;
} grNode;


typedef struct {
  grNode *nodes;
  grEdge *edges;
  int numnodes;
  int numedges;
} grGraph;


void grFree(grGraph *g);
grGraph* grAlloc(int numnodes, int numedges);
void grGenRandom(grGraph *g);
void graphDrawGL(grGraph *g);


#endif
