#ifndef _H_TEXTURE_
#define _H_TEXTURE_
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>

using namespace std;
class Texture{

	private:
		int height,width;
		string fn;
		unsigned char *data;
		int read_image();
	public:
		Texture(string filename);
		~Texture();
		int get_height();
		int get_width();
		unsigned char* get_data();
};

#endif