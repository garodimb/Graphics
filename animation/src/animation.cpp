#include <animation.h>
#include <log.h>

Animation::Animation()
{
	config_camera();
}

Animation::~Animation()
{
	delete cam_pos_space;
	delete cam_up_space;
	delete cam_lookat_space;

	delete cam_pos_obja;
	delete cam_up_obja;
	delete cam_lookat_obja;

	delete cam_pos_objb;
	delete cam_up_objb;
	delete cam_lookat_objb;

	delete cam_pos_objc;
	delete cam_up_objc;
	delete cam_lookat_objc;
}

int Animation::config_camera()
{
	cam_pos_space = new Vector;
	cam_lookat_space = new Vector;
	cam_up_space = new Vector;

	cam_pos_obja = new Vector;
	cam_lookat_obja = new Vector;
	cam_up_obja = new Vector;

	cam_pos_objb = new Vector;
	cam_lookat_objb = new Vector;
	cam_up_objb = new Vector;

	cam_pos_objc = new Vector;
	cam_lookat_objc = new Vector;
	cam_up_objc = new Vector;

	/* Camera in space */
	cam_pos_space->x = 2.0f;
	cam_pos_space->y = -1.0f;
	cam_pos_space->z = -4.0f;

	cam_up_space->x = 0.0f;
	cam_up_space->y = 1.0f;
	cam_up_space->z = 0.0f;

	cam_lookat_space->x = 0.0f;
	cam_lookat_space->y = -1.0f;
	cam_lookat_space->z = 0.0f;

	/* Camera on Model A */
	cam_pos_obja->x = 0.0f;
	cam_pos_obja->y = -1.0f;
	cam_pos_obja->z = -0.0f;

	cam_up_obja->x = 0.0f;
	cam_up_obja->y = 1.0f;
	cam_up_obja->z = 0.0f;

	cam_lookat_obja->x = 0.0f;
	cam_lookat_obja->y = -2.0f;
	cam_lookat_obja->z = 0.0f;

	/* Camera on Model B */
	cam_pos_objb->x = 0.0f;
	cam_pos_objb->y = -1.25f;
	cam_pos_objb->z = -0.70f;

	cam_up_objb->x = 0.0f;
	cam_up_objb->y = 1.0f;
	cam_up_objb->z = 0.0f;

	cam_lookat_objb->x = 0.0f;
	cam_lookat_objb->y = -2.0f;
	cam_lookat_objb->z = 0.0f;

	/* Camera on Model C */
	cam_pos_objc->x = 0.0f;
	cam_pos_objc->y = -1.15f;
	cam_pos_objc->z = -0.70f;

	cam_up_objc->x = 0.0f;
	cam_up_objc->y = 1.0f;
	cam_up_objc->z = 0.0f;

	cam_lookat_objc->x = 0.0f;
	cam_lookat_objc->y = -2.0f;
	cam_lookat_objc->z = 0.0f;
	return 0;
}

/* SPACE = 0, OBJ B = 1, OBJ C = 2 */
int Animation::get_camera(Vector& position,Vector& lookat,Vector &up,int obj)
{
	switch(obj){
		case 0:
			position.x = cam_pos_space->x; position.y = cam_pos_space->y; position.z = cam_pos_space->z;
			lookat.x = cam_lookat_space->x; lookat.y = cam_lookat_space->y; lookat.z = cam_lookat_space->z;
			up.x = cam_up_space->x; up.y = cam_up_space->y; up.z = cam_up_space->z;
			break;

		case 1:
			position.x = cam_pos_objb->x; position.y = cam_pos_objb->y; position.z = cam_pos_objb->z;
			lookat.x = cam_lookat_objb->x; lookat.y = cam_lookat_objb->y; lookat.z = cam_lookat_objb->z;
			up.x = cam_up_objb->x; up.y = cam_up_objb->y; up.z = cam_up_objb->z;
			break;

		case 2:
			position.x = cam_pos_objc->x; position.y = cam_pos_objc->y; position.z = cam_pos_objc->z;
			lookat.x = cam_lookat_objc->x; lookat.y = cam_lookat_objc->y; lookat.z = cam_lookat_objc->z;
			up.x = cam_up_objc->x; up.y = cam_up_objc->y; up.z = cam_up_objc->z;
			break;
		case 3:
			position.x = cam_pos_obja->x; position.y = cam_pos_obja->y; position.z = cam_pos_obja->z;
			lookat.x = cam_lookat_obja->x; lookat.y = cam_lookat_obja->y; lookat.z = cam_lookat_obja->z;
			up.x = cam_up_obja->x; up.y = cam_up_obja->y; up.z = cam_up_obja->z;
			break;
		default:
			log_E("Invalid Model");
			return 1;
		}
	return 0;
}