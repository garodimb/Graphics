#ifndef _VIEW_H
#define _VIEW_H

#include <quaternion.h>
#include <model.h>
#include <cube.h>

class View{

	private:
		int screen_width;
		int screen_height;
		int pos[2];
		int num_models;
		Quaternion fix_pos,curr_pos;
		Cube *cube;
		Model **model;
		GLfloat *track_matrix;
		GLfloat rotate_x; // X Rotation
		GLfloat rotate_y; // Y Rotation
		GLfloat z_distance; // Camera distance
		GLfloat scale_all; // Uniform scaling in all direction
		GLfloat trans_x; // X translation
		GLfloat trans_y; // Y translation
		GLfloat trans_z; // Z translation


	public:
		View(int argc,char **argv);
		~View();
		int init_window(const char *title,int w,int h);
		int init(void);
		void reshape(int w,int h);
		void display(void);
		int draw_axis();
		int reg_mouse_handler(void (*mouse_handler)(int button,int state,int x,int y));
		int reg_motion_handler(void (*motion_handler)(int x,int y));
		int reg_keyboard_handler(void (*keyboard_handler)(unsigned char key,int x,int y));
		int reg_keyboard_spec_handler(void (*keyboard_spec_handler)(int key,int x,int y));
		int rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);
		int set_camera();
		int set_fixed_light();
		int refresh(GLfloat rotate_x=0.0f, GLfloat rotate_y=0.0f, GLfloat z_distance = 5.0f,GLfloat scale_all=1.0f,GLfloat trans_x=0.0f,GLfloat trans_y=0.0f,GLfloat trans_z=0.0f,GLfloat *track_matrix=NULL);
		int get_width();
		int get_height();
		int init_lighting();

};

#endif
