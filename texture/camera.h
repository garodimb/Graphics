#ifndef _CAMERA_H
#define _CAMERA_H

#include <datatypes.h>

class Camera{
	private:
		Vector position;
		Vector lookat;
		Vector up;

	public:
		Camera();
		~Camera();
		Vector get_position();
		Vector get_lookat();
		Vector get_up();
};

#endif