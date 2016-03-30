#ifndef _CONTROLLER_H
#define _CONTROLLER_H


#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <quaternion.h>
#include <model.h>
#include <cube.h>
#include <trackball.h>
#include <view.h>

#define KEY_ENTER 13 //Switch Mode Key
#define KEY_ESC 27 //Exit Application Key
#define ZOOMIN 43
#define ZOOMOUT 45
#define KEY_L 108 // Left Rotation
#define KEY_R 114 // Right Rotation
#define KEY_H 104
#define KEY_V 118
#define KEY_D 100
#define KEY_x 120
#define KEY_X 88
#define KEY_y 121
#define KEY_Y 89
#define KEY_z 122
#define KEY_Z 90
#define KEY_S 83
#define KEY_s 115

/* Rotation Angle */
#define ROTATE_ANGLE 5.0
#define FL_DRAG 1
#define FL_PUSH 0


class Controller{

	public:
		Controller(View *view);
		~Controller();

		/* Callbacks */
		void mouse_handler(int button,int state,int x,int y);
		void motion_handler(int x,int y);
		void keyboard_handler(unsigned char key,int x,int y);
		void keyboard_spec_handler(int key,int x,int y);
		void trackball_handler(int event,int xx,int yy);

		/* Transformations */
		int trans_by_user(GLint x,GLint y);
		int reset();
		int refresh_view();

		Trackball *trackball;
		View *view;

	private:
		/* Transformations */
			GLint x_ang;
			GLint y_ang;
			GLfloat track_matrix[4][4];
			GLfloat rotate_x; // X Rotation
			GLfloat rotate_y; // Y Rotation
			GLfloat z_distance; // Camera distance
			GLfloat scale_all; // Uniform scaling in all direction
			GLfloat trans_x; // X translation
			GLfloat trans_y; // Y translation
			GLfloat trans_z; // Z translation
			GLint enable_roat;
	};

#endif
