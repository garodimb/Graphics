#ifndef _MODEL_H
#define _MODEL_H
#include <GL/glut.h>
#include <cmath>
#include <ply.h>
#include <control.h>
#include <iostream>

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

typedef struct TexCord{
	float u,v;
	}TexCord;

enum Map { Sphere, Cylinder };

class Model{

	public:
		Model(string& fn,Map map);
		~Model();
		int read_ply();
		int display();
	private:
		float x_min,x_max,y_min,y_max,z_min,z_max;
		float scale_factor;
		int nvertices,ntriangles;
		Vertex **vlist;
		Face **flist;
		Vector **normal;
		TexCord **tex_cord;
		GLuint tex_name;
		Vertex centroid;
		Map map;
		string fn;

		int compute_normal();
		int compute_scale_factor();
		int compute_sphere_cord();
		int compute_cyl_cord();
		int init_tex();
};

#endif
