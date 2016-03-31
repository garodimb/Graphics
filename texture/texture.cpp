#include <texture.h>

Texture::Texture(string& filename)
{
	fn = filename;
	read_image();
}

Texture::~Texture()
{

}

int Texture::read_image()
{
	image.read(fn.c_str());
	image.type(TrueColorType);
	width = image.size().width();
	height = image.size().height();
	pixels = image.getPixels(0,0,width,height);
	return 0;
}
int Texture::get_height()
{
	return height;
}

int Texture::get_width()
{
	return width;
}

GLfloat* Texture::get_pixels()
{
	return pixels;
}