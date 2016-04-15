#ifndef _DATATYPE_H
#define _DATATYPE_H

typedef struct Vertex {
  float x,y,z;             /* the usual 3-space position of a vertex */
} Vertex;

typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face;

typedef struct Vector {
	float x,y,z;
} Vector;

typedef struct TexCord{
	float u,v;
	}TexCord;

enum Map { Sphere, Cylinder };

#endif