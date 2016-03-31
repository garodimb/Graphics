#ifndef _H_TEXTURE_
#define _H_TEXTURE_
#include <iostream>
#include <GL/glut.h>
#include <Magick++.h>

using namespace std;
using namespace Magick;
class Texture{

	private:
		int height,width;
		string fn;
		GLfloat *pixels;
		Image image;
		int read_image();
	public:
		Texture(string& filename);
		~Texture();
		int get_height();
		int get_width();
		GLfloat* get_pixels();
};

#endif