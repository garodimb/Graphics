#ifndef _CUBE_H
#define _CUBE_H
#define NUM_FACES 6

class Cube{
	public:
		GLfloat v[8][3];
		GLfloat c[8][3];
		GLfloat n[6][3];
		GLint faces[6][4];
		Cube (float dim);
		Cube ();
		GLint display ();
	};

#endif
