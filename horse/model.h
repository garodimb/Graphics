#ifndef _MODEL_H
#define _MODEL_H
#include <GL/glut.h>
#include "rply/rply.h"
#include "control.h"

typedef struct vec3{
	double x;
	double y;
	double z;
	}vec3;

typedef struct ivec3{
	int x;
	int y;
	int z;
	}ivec3;
	
typedef vec3 vertex;
typedef vec3 color;
typedef ivec3 triangle;

class Model{
	
	public:
		static long nvertices,ntriangles;
		static long count_v, count_t;
		static vertex *vertices;
		static triangle *triangles;
		char filename[100];
		
		static int display();
		static int init(char *filename);
	private:
		static int face_cb(p_ply_argument argument);
		static int vertex_cb(p_ply_argument argument);
};

#endif
