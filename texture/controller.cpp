#include <controller.h>

Controller *controller;


static void reg_mouse_handler(int button,int state,int x,int y){
	controller->mouse_handler(button,state,x,y);
	}

static void reg_motion_handler(int x,int y){
	controller->motion_handler(x,y);
	}

static void reg_keyboard_handler(unsigned char key,int x,int y){
	controller->keyboard_handler(key,x,y);
	}

static void reg_keyboard_spec_handler(int key,int x,int y){
	controller->keyboard_spec_handler(key,x,y);
	}

Controller::Controller(View *view){
		x_ang = 0;
		y_ang = 0;
		rotate_x 	= 0.0; // X Rotation
		rotate_y 	= 0.0; // Y Rotation
		z_distance 	= 0.0f; // Camera distance relative to current position
		scale_all	= 1.0f; // Uniform scaling in all direction
		trans_x  	= 0.0f; // X translation
		trans_y  	= 0.0f; // Y translation
		trans_z  	= 0.0f; // Z translation
		light_status[0] = light_status[1] = light_status[2] = light_status[3] = true;
		memset(track_matrix,0x00,sizeof(track_matrix));
		track_matrix[0] = track_matrix[1] = track_matrix[2] = 0.0f;
		track_matrix[3] = 1.0f;
		trackball = new Trackball();
		this->view = view;
		controller = this;
		view->reg_mouse_handler(reg_mouse_handler);
		view->reg_motion_handler(reg_motion_handler);
		view->reg_keyboard_handler(reg_keyboard_handler);
		view->reg_keyboard_spec_handler(reg_keyboard_spec_handler);

	}
Controller::~Controller(){
	delete trackball;
	}
/* Mouse Handler */
void Controller::mouse_handler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON){
		enable_roat=1;
		trackball_handler(FL_PUSH,x,y);
		}
	else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
		//Do Unprojection here
		enable_roat=0;
		trans_by_user(x,y);
		}
	}

/* Mouse motion Handler */
void Controller::motion_handler(int x, int y){
	if(enable_roat==1)
		trackball_handler(FL_DRAG,x,y);
	}

/* Keyboard Handler */
void Controller::keyboard_handler(unsigned char key,int x,int y){
	log_D("Key Pressed: " << (int)key);
	if(key==KEY_ESC){
		log_I("ESC key pressed. Exiting Program");
		exit(0);
		}
	else if(key==KEY_ENTER){
		log_I("Resetting MODELVIEW");
		reset();
		refresh_view();
		}
	else if(key==ZOOMIN){
		log_I("Moving Camera Near");
		z_distance-=0.1;
		refresh_view();
		}
	else if(key==ZOOMOUT){
		log_I("Moving Camera far");
		z_distance+=0.1;
		refresh_view();
		}
	else if(key == KEY_X){
		log_I("Translating +X");
		trans_x += 0.1f;
		refresh_view();
		}
	else if(key == KEY_x){
		log_I("Translating -X");
		trans_x -= 0.1f;
		refresh_view();
		}
	else if(key == KEY_Y){
		log_I("Translating +Y");
		trans_y += 0.1f;
		refresh_view();
		}
	else if(key == KEY_y){
		log_I("Translating -Y");
		trans_y -= 0.1f;
		refresh_view();
		}
	else if(key == KEY_Z){
		log_I("Translating +Z");
		trans_z += 0.1f;
		refresh_view();
		}
	else if(key == KEY_z){
		log_I("Translating -Z");
		trans_z -= 0.1f;
		refresh_view();
		}
	else if(key == KEY_S){
		log_I("Scaling +X, +Y, +Z");
		scale_all += 0.1f;
		refresh_view();
		}
	else if(key == KEY_s){
		log_I("Scaling -X, -Y, -Z");
		scale_all -= 0.1f;
		refresh_view();
		}
	else if(key == KEY_1 || KEY_2 || KEY_3){
		light_status[key-KEY_1]= !light_status[key-KEY_1];
		refresh_view();
		}
	}

/* Keyboard Special keys handler */
void Controller::keyboard_spec_handler(int key,int x,int y){
	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT){
		rotate_y += ROTATE_ANGLE;
		}
	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT){
		rotate_y -= ROTATE_ANGLE;
	}
	else if (key == GLUT_KEY_UP){
		rotate_x += ROTATE_ANGLE;
	}
	else if (key == GLUT_KEY_DOWN){
		rotate_x -= ROTATE_ANGLE;
	}
	//  Request display update
	refresh_view();
	}

/* Trackball Handler */
void Controller::trackball_handler(int event,int xx,int yy){
  float screen_height,screen_width;
  screen_height = (float) view->get_height();
  screen_width = (float) view->get_width();
	if ( event == FL_PUSH ) {
      x_ang = xx ;
      y_ang = yy ;
      trackball->rotate( 0,0,0,0 ) ;
    }
    else if (( event == FL_DRAG ) ) {
		trackball->rotate((2.0 * x_ang - screen_width) / screen_width,
             (screen_height - 2.0 * y_ang) / screen_height,
             (2.0 * xx - screen_width) / screen_width,
             (screen_height - 2.0 * yy) / screen_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    trackball->get_quaternion(track_matrix);
    x_ang = xx ; y_ang = yy ;
    refresh_view() ;
    }
}

/* Do Translation by user defined distance */
int Controller::trans_by_user(GLint x,GLint y){
	GLfloat x_dist,y_dist,dist;
	static GLint t_point_count = -1;
	static Vertex trans_point;
	if(t_point_count==-1){
		/* Add first point for finding translation distance */
		trans_point.x = x;
		trans_point.y = y;
		trans_point.z = 0;
		t_point_count = 0;
		}
	else if(t_point_count==0){
		/* Calculate distance between two points */
		dist = sqrt(pow(trans_point.x - x, 2)+ pow(trans_point.y - y, 2));
		t_point_count = -1;

		/* Set X & Y direction */
		x_dist = ( x >= trans_point.x)? dist : -dist;
		y_dist = ( y >= trans_point.y)? dist : -dist;
		log_I("Translating");
		log_D("x_dist: "<<x_dist<<", y_dist: "<<y_dist);
		trans_x += (float )x_dist/view->get_width();
		trans_y += (float )-y_dist/view->get_height();
		refresh_view();
		}
	return 0;
	}

/* Reset all transformations */
int Controller::reset(){
	x_ang 		= 0; // Trackball Angles
	y_ang 		= 0; //Trackbal Angles

	rotate_x 	= 0.0f; // X Rotation
	rotate_y 	= 0.0f; // Y Rotation
	z_distance 	= 0.0f; // Camera distance relative to current position
	scale_all	= 1.0f; // Uniform scaling in all direction
	trans_x		= 0.0f; // X translation
	trans_y		= 0.0f; // Y translation
	trans_z		= 0.0f; // Z translation
	memset(track_matrix,0x00,sizeof(track_matrix));
	track_matrix[0] = track_matrix[1] = track_matrix[2] = 0.0f;
	track_matrix[3] = 1.0f;
	light_status[0] = light_status[1] = light_status[2] = light_status[3] = true;
	delete trackball;
	trackball = new Trackball();
	}

int Controller::refresh_view(){
	view->refresh(rotate_x,rotate_y,z_distance,scale_all,trans_x,trans_y,trans_z,(GLfloat *)track_matrix,light_status);
	return 0;
	}
