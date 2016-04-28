
#include <datatypes.h>
#include <iostream>
using namespace std;
class Animation
{
	private:
		/* Camera positions */
		Vector *cam_pos_space;
		Vector *cam_up_space;
		Vector *cam_lookat_space;

		Vector *cam_pos_obja;
		Vector *cam_up_obja;
		Vector *cam_lookat_obja;

		Vector *cam_pos_objb;
		Vector *cam_up_objb;
		Vector *cam_lookat_objb;

		Vector *cam_pos_objc;
		Vector *cam_up_objc;
		Vector *cam_lookat_objc;

		/* Model Transformations */
		Vector *trans_obj_a;
		Vector *scale_obj_a;
		float angle_obj_a;

		Vector *trans_obj_b;
		Vector *scale_obj_b;
		float angle_obj_b;

		Vector *trans_obj_c;
		Vector *scale_obj_c;
		float angle_obj_c;

		/* Spotlight position and direction */
		Vector *spot_pos;
		Vector *spot_direction;

		int config_camera();
		int config_model();
		int config_light();

	public:
		Animation();
		int get_camera(Vector& position,Vector& lookat,Vector &curr,int obj);
		int get_model_transf(Vector& trans, Vector& scale,float& angle,int obj);
		int set_spot_light(Vector& pos, Vector& direction);
		int get_spot_light(Vector& pos, Vector& direction);
		~Animation();

};