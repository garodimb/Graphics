#ifndef _VIEW_H
#define _VIEW_H

#include <quaternion.h>
#include <model.h>
#include <cube.h>
#include <camera.h>
#include <scenenode.h>
#include <matrix.h>
#include <animation.h>

/* Camera  location */
#define IN_SPACE 0
#define ON_OBJ_B 1
#define ON_OBJ_C 2
#define ON_OBJ_A 3

class View{

	private:
		int screen_width;
		int screen_height;
		int pos[2];
		int num_models;
		int num_nodes;
		int cam_loc; //Where is camera w.r.t. model
		Cube *cube;
		SceneNode *scene;
		SceneNode **node; //Pointers to all scenenode, convert this into vector
		Model **model; //Pointers to all model, convert this into vector
		Camera *camera;
		Animation *animation;
		GLfloat *track_matrix;
		bool light_status[6];
		int curr_obj;
		bool do_detach;
		bool do_attach;
		bool enable_animation;
		float cow_speed, train_speed;

	public:
		View(int argc,char **argv);
		~View();
		int init_window(const char *title,int w,int h);
		int init(void);
		int init_scene(int argc,char **argv);
		void reshape(int w,int h);
		void display(void);
		void idle_func_handler(void);
		int draw_axis();
		int reg_mouse_handler(void (*mouse_handler)(int button,int state,int x,int y));
		int reg_motion_handler(void (*motion_handler)(int x,int y));
		int reg_keyboard_handler(void (*keyboard_handler)(unsigned char key,int x,int y));
		int reg_keyboard_spec_handler(void (*keyboard_spec_handler)(int key,int x,int y));
		int set_camera();
		int set_fixed_light();
		int set_headlight();
		int set_spotlight(Vector& pos,Vector &direction);
		int refresh(GLfloat *track_matrix=NULL,bool *light_status=NULL,int cam_loc=IN_SPACE,bool enable_animation=true,float train_speed=0.02,float cow_speed=0.02);
		int get_width();
		int get_height();
		int init_lighting();
		int detach_node();
		int update_tex(string &tex_path,int obj);
		int update_tex_mode(int mode,int obj);
		int init_camera(void);
		int update_trans_obj(float trans_x,float trans_z,int curr_obj);

};

#endif
