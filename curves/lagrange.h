#ifndef CONTROL_H
#define CONTROL_H
#include "control.h"
#endif

class Lagrange{
	public:
		Point *user_points;
		Point *comp_points;
		GLint num_points;
		GLint num_comp_points;
		
		Lagrange(int n,Point *user_points);
		GLint compute(GLint num_lag_points);
		GLint compute_coeff(GLfloat *coeff);
		GLint compute_point(GLfloat x,const GLfloat *coeff,Point *p);
		GLint display();
	};
