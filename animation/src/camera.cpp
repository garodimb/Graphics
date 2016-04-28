
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

/* Only sets current position of camera */
int Camera::config_camera(Vector& position, Vector &lookat, Vector &up)
{
	curr_pos = position;
	curr_lookat = lookat;
	curr_up = up;
	return 0;
}

/* Return current camera position */
Vector Camera::get_position()
{
	return curr_pos;
}

/* Return current lookat vector */
Vector Camera::get_lookat()
{
	return curr_lookat;
}

/* Return current up vector */
Vector Camera::get_up()
{
	return curr_up;
}

/* Rotate camera wrt initial position */
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

	curr_lookat = fix_lookat;
	curr_up = fix_up;
	return 0;
}

/* Set camera by calling gluLookAt on current position */
int Camera::set_camera()
{
	gluLookAt(curr_pos.x, curr_pos.y,curr_pos.z, curr_lookat.x, curr_lookat.y,curr_lookat.z, curr_up.x, curr_up.y, curr_up.z);
	return 0;
}