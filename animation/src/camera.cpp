
#include <camera.h>
#include <GL/glut.h>

Camera::Camera(Vector& position, Vector& lookat, Vector &up)
{
	fix_pos = curr_pos = position;
	fix_lookat = curr_lookat = lookat;
	fix_up = curr_up = up;
}

Camera::~Camera()
{}

Vector Camera::get_position()
{
	return curr_pos;
}


Vector Camera::get_lookat()
{
	return curr_lookat;
}

Vector Camera::get_up()
{
	return curr_up;
}

int Camera::rotate_camera(Quaternion q)
{
	Quaternion pos;
	pos.x = fix_pos.x;
	pos.y = fix_pos.y;
	pos.z = fix_pos.z;
	pos.w = 0.0f;

	pos = pos * q;
	log_D("Current camera position: "<<pos);
	curr_pos.x = pos.x;
	curr_pos.y = pos.y;
	curr_pos.z = pos.z;

	return 0;
}

int Camera::set_camera()
{
	gluLookAt(curr_pos.x, curr_pos.y,curr_pos.z, curr_lookat.x, curr_lookat.y,curr_lookat.z, curr_up.x, curr_up.y, curr_up.z);
	return 0;
}