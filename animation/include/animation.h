
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

		Vector *cam_pos_objb;
		Vector *cam_up_objb;
		Vector *cam_lookat_objb;

		Vector *cam_pos_objc;
		Vector *cam_up_objc;
		Vector *cam_lookat_objc;

		int config_camera();

	public:
		Animation();
		int get_camera(Vector& position,Vector& lookat,Vector &curr,int obj);
		~Animation();

};