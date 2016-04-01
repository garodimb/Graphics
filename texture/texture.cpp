#include <texture.h>

Texture::Texture(string filename)
{
	fn = filename;
	read_image();
}

Texture::~Texture()
{
	delete [] data;
}

int Texture::read_image()
{
	int i;
	FILE* f = fopen(fn.c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	width = *(int*)&info[18];
	height = *(int*)&info[22];

	int size = 3 * width * height;
	data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	for(i = 0; i < size; i += 3)
	{
		unsigned char tmp = data[i];
		data[i] = data[i+2];
		data[i+2] = tmp;
	}

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

unsigned char* Texture::get_data()
{
	return data;
}