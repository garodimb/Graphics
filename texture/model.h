#ifndef _MODEL_H
#define _MODEL_H
#include <GL/glut.h>
#include <cmath>
#include <ply.h>
#include <control.h>
#include <datatypes.h>
#include <iostream>

using namespace std;

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
