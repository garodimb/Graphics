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
		init();
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

int Controller::init()
{
	x_ang = 0;
	y_ang = 0;
	curr_obj	= 0;
	cam_loc		= IN_SPACE;
	light_status[0] = light_status[1] = light_status[2] = light_status[3] = true;
	light_status[4]	= light_status[5] = true;
	memset(track_matrix,0x00,sizeof(track_matrix));
	track_matrix[0] = track_matrix[1] = track_matrix[2] = 0.0f;
	track_matrix[3] = 1.0f;
	enable_animation = true;
	train_speed = cow_speed = 0.01;
	return 0;
}

/* Mouse Handler */
void Controller::mouse_handler(int button,int state,int x,int y){
	if((button==GLUT_LEFT_BUTTON) && (cam_loc == IN_SPACE)){
		enable_roat=1;
		trackball_handler(FL_PUSH,x,y);
		}
	else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
		enable_roat=0;
		set_curr_obj(x,y);
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
		log_D("Resetting MODELVIEW");
		reset();
		refresh_view();
		}
	else if(KEY_1 <= key && key <= KEY_6){
		log_I("Toggeling light");
		light_status[key-KEY_1]= !light_status[key-KEY_1];
		refresh_view();
		}
	else if(key == KEY_T || key == KEY_t){
		log_I("Changing texture");
		static int pos = 0;
		pos++;
		string fn[] = {"texfiles/floor.bmp","texfiles/apple.bmp","texfiles/apple2.bmp","texfiles/canstick.bmp","texfiles/ant.bmp","texfiles/cow.bmp"};
		view->update_tex(fn[pos%(sizeof(fn)/sizeof(fn[0]))],curr_obj);
		}
	else if(key == KEY_A || key == KEY_a){
		log_I("Texture mapping OpenGL mode");
		view->update_tex_mode(0,curr_obj);
		}
	else if(key == KEY_M || key == KEY_m){
		log_I("Texture mapping manual mode");
		view->update_tex_mode(1,curr_obj);
		}
	else if(key == KEY_S || key == KEY_s){
		log_I("Placing camera in space");
		cam_loc = IN_SPACE;
		refresh_view();
		}
	else if(key == KEY_Y || key == KEY_y){
		log_I("Placing camera on object B");
		cam_loc = ON_OBJ_B;
		refresh_view();
		}
	else if(key == KEY_Z || key == KEY_z){
		log_I("Placing camera on object C");
		cam_loc = ON_OBJ_C;
		refresh_view();
		}
	else if(key == KEY_X || key == KEY_x){
		log_I("Placing camera on object A");
		cam_loc = ON_OBJ_A;
		refresh_view();
		}
	else if(key == KEY_D || key == KEY_d){
		log_I("Request for detach");
		view->detach_node();
		}
	else if(key == KEY_SPACE){
		log_I("Toggling animation");
		enable_animation =!enable_animation;
		refresh_view();
		}
	}

/* Keyboard Special keys handler */
void Controller::keyboard_spec_handler(int key,int x,int y){
	if(key == GLUT_KEY_UP){
		train_speed+=0.01;
		if(train_speed>=0.5){
			train_speed = 0.5;
			}
		}
	else if( key == GLUT_KEY_DOWN){
		train_speed-=0.01;
		if(train_speed<=0){
			train_speed = 0;
			}
		}
	else if(key == GLUT_KEY_RIGHT){
		cow_speed+=0.01;
		if(cow_speed>=0.5){
			cow_speed = 0.5;
			}
		}
	else if(key == GLUT_KEY_LEFT){
		cow_speed-=0.01;
		if(cow_speed<=0){
			cow_speed=0;
			}
		}
	refresh_view();
	return ;
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
    /* Get qaurternion */
    trackball->get_quaternion(track_matrix);
    x_ang = xx ; y_ang = yy ;
    refresh_view() ;
    }
}

/* Reset all transformations */
int Controller::reset(){
	init();
	trackball = new Trackball();
	}

int Controller::refresh_view(){
	view->refresh((GLfloat *)track_matrix,light_status,cam_loc,enable_animation,train_speed,cow_speed);
	return 0;
	}

int Controller::get_curr_obj()
{
	return curr_obj;
}

int Controller::set_curr_obj(int x,int y)
{
	GLint index;
	GLfloat screen_height = (float) view->get_height();
	glReadPixels(x, screen_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
	log_D("Selected object: "<<index);
	curr_obj = index;
	return 0;
}