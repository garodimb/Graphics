#ifndef CONTROL_H
#define CONTROL_H
#include "control.h"
#endif

#define NUM_FACES 6

class Cube{
	public:
		GLfloat v[8][3];
		GLfloat c[8][3];
		GLfloat n[6][3];
		GLint faces[6][4];
		Cube();
		GLint display();
	};
