
#include <camera.h>

Camera::Camera(Vector& position, Vector& lookat,Vector &up)
{
	this->position = position;
	this->lookat = lookat;
	this->up = up;
}

Camera::Camera()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = -5.0f;

	lookat.x = lookat.y = lookat.z = 0.0f;

	up.x = up.z = 0.0f;
	up.y = 1.0f;
}

Camera::~Camera()
{}

Vector Camera::get_position()
{
	return position;
}


Vector Camera::get_lookat()
{
	return lookat;
}


Vector Camera::get_up()
{
	return up;
}