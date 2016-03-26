#ifndef _MODEL_H
#define _MODEL_H
#include <GL/glut.h>
#include "rply/ply.h"
#include "control.h"

using namespace std;

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

class Model{

	public:
		Model();
		~Model();
		int read_ply(char *fn);
		int display();
	private:
		int compute_normal();
		float x_min,x_max,y_min,y_max,z_min,z_max;
		int nvertices,ntriangles;
		Vertex **vlist;
		Face **flist;
		Vector **normal;
};

#endif
