#include <animation.h>
#include <log.h>

Animation::Animation()
{
	config_camera();
	config_model();
	config_light();
	do_attach = false;
	do_detach = false;
}

Animation::~Animation()
{
	delete cam_pos_space; delete cam_up_space; delete cam_lookat_space;
	delete cam_pos_obja; delete cam_up_obja; delete cam_lookat_obja;
	delete cam_pos_objb; delete cam_up_objb; delete cam_lookat_objb;
	delete cam_pos_objc; delete cam_up_objc; delete cam_lookat_objc;

	delete trans_obj_a;	delete scale_obj_a;
	delete trans_obj_b;	delete scale_obj_b;
	delete trans_obj_c;	delete scale_obj_c;

	delete spot_pos, delete spot_direction;
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
	cam_pos_space->x = 2.0f; cam_pos_space->y = -1.0f; cam_pos_space->z = -4.0f;
	cam_up_space->x = 0.0f; cam_up_space->y = 1.0f; cam_up_space->z = 0.0f;
	cam_lookat_space->x = 0.0f; cam_lookat_space->y = -1.0f; cam_lookat_space->z = 0.0f;

	/* Camera on Model A */
	cam_pos_obja->x = 0.0f; cam_pos_obja->y = -1.0f; cam_pos_obja->z = -0.0f;
	cam_up_obja->x = 0.0f; cam_up_obja->y = 1.0f; cam_up_obja->z = 0.0f;
	cam_lookat_obja->x = 0.0f; cam_lookat_obja->y = -2.0f; cam_lookat_obja->z = 0.0f;

	/* Camera on Model B */
	cam_pos_objb->x = 0.0f; cam_pos_objb->y = -1.25f; cam_pos_objb->z = -0.70f;
	cam_up_objb->x = 0.0f; cam_up_objb->y = 1.0f; cam_up_objb->z = 0.0f;
	cam_lookat_objb->x = 0.0f; cam_lookat_objb->y = -2.0f; cam_lookat_objb->z = 0.0f;

	/* Camera on Model C */
	cam_pos_objc->x = 0.0f;	cam_pos_objc->y = -1.15f; cam_pos_objc->z = -0.70f;
	cam_up_objc->x = 0.0f; cam_up_objc->y = 1.0f; cam_up_objc->z = 0.0f;
	cam_lookat_objc->x = 0.0f; cam_lookat_objc->y = -2.0f; cam_lookat_objc->z = 0.0f;
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

int Animation::config_model()
{
	trans_obj_a = new Vector;
	scale_obj_a = new Vector;

	trans_obj_b = new Vector;
	scale_obj_b = new Vector;

	trans_obj_c = new Vector;
	scale_obj_c = new Vector;

	trans_obj_a->x = 0.0f; trans_obj_a->y = -1.7f; trans_obj_a->z = 0.0f;
	scale_obj_a->x = 0.15f; scale_obj_a->y = 0.15f; scale_obj_a->z = 0.15f;
	angle_obj_a = 0.0f;

	trans_obj_b->x = 0.058f; trans_obj_b->y = -1.27f; trans_obj_b->z = -0.35f;
	scale_obj_b->x = 0.1f; scale_obj_b->y = 0.1f; scale_obj_b->z = 0.1f;
	angle_obj_b = 0.0f;

	trans_obj_c->x = 0.07f; trans_obj_c->y = -1.25f; trans_obj_c->z = -0.35f;
	scale_obj_c->x = 0.006f; scale_obj_c->y = 0.006f; scale_obj_c->z = 0.006f;
	angle_obj_c = 270.0f;
	return 0;
}

int Animation::get_model_transf(Vector& trans, Vector& scale,float &angle,int obj)
{
	switch(obj){
		case 1:
			trans.x = trans_obj_a->x; trans.y = trans_obj_a->y; trans.z = trans_obj_a->z;
			scale.x = scale_obj_a->x; scale.y = scale_obj_a->y; scale.z = scale_obj_a->z;
			angle = angle_obj_a;
			break;

		case 2:
			trans.x = trans_obj_b->x; trans.y = trans_obj_b->y; trans.z = trans_obj_b->z;
			scale.x = scale_obj_b->x; scale.y = scale_obj_b->y; scale.z = scale_obj_b->z;
			angle = angle_obj_b;
			break;

		case 3:
			trans.x = trans_obj_c->x; trans.y = trans_obj_c->y; trans.z = trans_obj_c->z;
			scale.x = scale_obj_c->x; scale.y = scale_obj_c->y; scale.z = scale_obj_c->z;
			angle = angle_obj_c;
			break;
		default:
			log_E("Invalid Model");
			return 1;
		}
	return 0;
}

int Animation::config_light()
{
	spot_pos = new Vector;
	spot_direction = new Vector;

	spot_pos->x = 0.0f; spot_pos->y = -0.5f; spot_pos->z = 0.0f;
	spot_direction->x = 0.0f; spot_direction->y = -0.5f; spot_direction->z = 0.0f;
	return 0;
}

int Animation::set_spot_light(Vector& pos, Vector& direction)
{
	spot_pos->x = pos.x; spot_pos->y = pos.y; spot_pos->z = pos.z;
	spot_direction->x = direction.x; spot_direction->y = direction.y; spot_direction->z = direction.z;
	return 0;
}

int Animation::get_spot_light(Vector& pos, Vector& direction)
{
	pos.x = spot_pos->x; pos.y = spot_pos->y; pos.z = spot_pos->z;
	direction.x = spot_direction->x; direction.y = spot_direction->y; direction.z = spot_direction->z;
	return 0;
}