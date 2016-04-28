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
		GLfloat rotate_x; // X Rotation
		GLfloat rotate_y; // Y Rotation
		GLfloat z_distance; // Camera distance
		GLfloat scale_all; // Uniform scaling in all direction
		GLfloat trans_x; // X translation
		GLfloat trans_y; // Y translation
		GLfloat trans_z; // Z translation
		bool light_status[6];
		int curr_obj;


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
		int rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);
		int set_camera();
		int set_fixed_light();
		int set_headlight();
		int set_spotlight(Vector& pos,Vector &direction);
		int refresh(GLfloat rotate_x=0.0f, GLfloat rotate_y=0.0f, GLfloat z_distance = 5.0f,GLfloat scale_all=1.0f,GLfloat trans_x=0.0f,GLfloat trans_y=0.0f,GLfloat trans_z=0.0f,GLfloat *track_matrix=NULL,bool *light_status=NULL,int cam_loc=IN_SPACE);
		int get_width();
		int get_height();
		int init_lighting();
		int update_tex(string &tex_path,int obj);
		int update_tex_mode(int mode,int obj);
		int init_camera(void);
		int update_trans_obj(float trans_x,float trans_z,int curr_obj);

};

#endif
