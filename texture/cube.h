#ifndef _CUBE_H
#define _CUBE_H
#define NUM_FACES 6


#define FRONT 	0
#define BACK 	1
#define RIGHT 	2
#define LEFT 	3
#define TOP 	4
#define BOTTOM 	5

#include <texture.h>
class Cube{
	private:
		GLfloat v[8][3];
		GLfloat c[8][3];
		GLfloat n[6][3];
		GLint faces[6][4];
		Texture *walls;
		Texture *floor;
		Texture *ceiling;
		string tex_path_floor;
		GLuint tex_name[3];
		int init_tex();
	public:
		Cube (float dim);
		Cube ();
		~Cube();
		GLint display ();
		int update_tex(string &tex_path);
	};

#endif
