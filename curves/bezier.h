#ifndef CONTROL_H
#define CONTROL_H
#include "control.h"
#endif

struct Point {
	GLfloat x;
	GLfloat y;
};


class Bezier{
	public:
		Point *user_points;
		Point *comp_points;
		GLint num_points;
		GLint num_comp_points;
		
		Bezier(int n,Point *user_points);
		GLint compute(GLint num_bez_points);
		GLint compute_coeff(GLfloat *coeff);
		GLint compute_point(GLfloat t,const GLfloat *coeff,Point *p);
		GLint display();
	};
