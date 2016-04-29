#include <iostream>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>
#include <view.h>

View *view;

static void on_display(){
	view->display();
	}

static void on_reshape(int w,int h){
	view->reshape(w,h);
	}

static void on_idle_func_handler()
{
	view->idle_func_handler();
}

/* Constructor */
View::View(int argc,char **argv){
	track_matrix= NULL; // Trackball Matrix
	light_status[0] = light_status[1] = light_status[2] = light_status[3] = true;
	light_status[4] = light_status[5] = true;
	enable_animation = true;
	cow_speed = train_speed = 0.01;
	glutInit(&argc,argv);
	init_window("Texture Mapping",1000,500);
	init();
	animation = new Animation();
	init_camera();
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutIdleFunc(on_idle_func_handler);
	view = this;
	cube = new Cube(4.0f);
	init_scene(argc,argv);
	}

/* Desctructor */
View::~View()
{
	for(int i = 0;i<num_models;i++)
		delete model[i];
	delete [] model;
	delete camera;
	delete scene;
	delete animation;
}

/* Initialize windowing system */
int View::init_window(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	glutInitWindowPosition(pos[0],pos[1]);
	glutCreateWindow(title);
	return 0;
	}

/* Init canvas */

int View::init_lighting(){

	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_diffuse[] = {0.8,0.8,0.8,1.0};
	GLfloat mat_shininess[] = { 128.0 };

	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_diffuse1[] = { 1.0, 0.0, 1.0, 0.0 };
	GLfloat light_diffuse2[] = { 0.0, 1.0, 1.0, 0.0 };
	GLfloat qaDiffuseLight[] = {1, 0, 1, 1.0};

	/* Disable LIGHTING to view only colored cube with different colors*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, qaDiffuseLight);
	return 0;
	}

/*
 * Initialize camera position and vector
 */
int View::init_camera(void)
{
	Vector cam_pos,cam_up,cam_lookat;
	cam_loc = IN_SPACE;
	animation->get_camera(cam_pos,cam_lookat,cam_up,IN_SPACE);
	delete camera;
	camera = new Camera(cam_pos,cam_lookat,cam_up);
	return 0;
}

int View::init(void){

	glClearColor(0.0,0.0,0.0,0.0);
	glClearStencil(0);
	glEnable(GL_NORMALIZE); //Dont care about performance now
	glShadeModel (GL_SMOOTH);
	init_lighting();
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	return 0;
	}

/*
 * Initialize scene
 */
int View::init_scene(int argc,char **argv){
	string fn1_obj,fn2_obj,fn3_obj,tex1_path,tex2_path,tex3_path;
	float matrix[16];
	Matrix mat;
	Vector trans,scale;
	float angle;
	num_models = 3;
	model = new Model*[num_models];
	node = new SceneNode*[num_nodes];
	scene = new SceneNode();
	fn1_obj = "plyfiles/train.ply";
	fn2_obj = "plyfiles/cow.ply";
	fn3_obj = "plyfiles/ant.ply";
	tex1_path = "texfiles/canstick.bmp";
	tex2_path = "texfiles/canstick.bmp";
	tex3_path = "texfiles/canstick.bmp";
	if(argc>=2){
		fn1_obj = argv[1];
		}
	if(argc>=3){
		fn2_obj = argv[2];
		}
	if(argc>=4){
		fn3_obj = argv[3];
		}
	if(argc>=5){
		tex1_path = argv[4];
		}
	if(argc>=6){
		tex1_path = argv[5];
		}

	 /* Create Models */
	model[0] = new Model(fn1_obj,Cylinder,tex1_path);
	model[1] = new Model(fn2_obj,Sphere,tex2_path);
	model[2] = new Model(fn3_obj,Cylinder,tex3_path);
	node[0] = new SceneNode();
	node[1] = new SceneNode();
	node[2] = new SceneNode();

	/* Train */
	node[0]->set_model(model[0]);
	animation->get_model_transf(trans,scale,angle,1);
	mat.get_Tmat(trans.x,trans.y,trans.z,matrix);
	node[0]->set_local_transf(matrix);
	mat.get_Smat(scale.x,scale.y,scale.z,matrix);
	node[0]->update_local_transf(matrix);

	/* Cow on Train */
	node[1]->set_model(model[1]);
	animation->get_model_transf(trans,scale,angle,2);
	mat.get_Tmat(trans.x,trans.y,trans.z,matrix);
	node[1]->set_local_transf(matrix);
	mat.get_Smat(scale.x,scale.y,scale.z,matrix);
	node[1]->update_local_transf(matrix);


	/* Ant on Cow */
	node[2]->set_model(model[2]);
	animation->get_model_transf(trans,scale,angle,3);
	mat.get_Tmat(trans.x,trans.y,trans.z,matrix);
	node[2]->set_local_transf(matrix);
	mat.get_Smat(scale.x,scale.y,scale.z,matrix);
	node[2]->update_local_transf(matrix);
	mat.get_Rmat(0.0,1.0,0.0,angle,matrix);
	node[2]->update_local_transf(matrix);


	/* Connected scene */
	scene->add_child(node[0]);
	node[0]->add_child(node[1]);
	node[1]->add_child(node[2]);

	do_detach = do_attach = false;
	return 0;
}

int View::get_width(){

	return screen_width;
	}

int View::get_height(){

	return screen_height;
	}

/* Reshape Handler */
void View::reshape(int w,int h){
	log_D("Handling reshape");
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, (float)w/h, 0.1f,15.0f);
	log_D("Window Width: "<<w<<", Height: "<<h);
	screen_height=h;
	screen_width=w;
	}

/* Display Handler */
void View::display(){
	log_D("Displaying content");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	set_headlight();
	set_camera();
	set_fixed_light();
	/* Move camera using z so to get fill of zooming */
	/* Eye, Center and Up vector */
	glStencilFunc(GL_ALWAYS,1,-1);
	cube->display();
	scene->display();
	glutSwapBuffers();
	}

void View::idle_func_handler(void)
{
	if(!enable_animation)
		return;
	Matrix mat;
	float matrix[16];
	static int direction = 0, cow_direction = 0;
	static float padding = 1.7;
	static float x = 1.7, z = 1.7, cow_x, cow_z;
	static int angle = 90, cow_angle = 90;
	static bool attach = true;
	static bool cow_reached = false;
	if(do_detach){
		node[1]->detach();
		scene->add_child(node[1]);
		if(direction==0){
			cow_x = x;
			cow_z = z + 0.70;
		}
		else if(direction==1){
			cow_x = x - 0.70;
			cow_z = z;
			}
		else if(direction==2){
			cow_x = x;
			cow_z = z - 0.70;
			}
		else{
			cow_x = x + 0.70;
			cow_z = z;
			}
		mat.get_Tmat(cow_x,-0.53,cow_z,matrix);
		node[1]->set_world_transf(matrix);
		mat.get_Rmat(0,1,0,angle,matrix);
		node[1]->update_world_transf(matrix);
		cow_direction = direction;
		cow_reached = false;
		attach = false;
		do_detach = false;
		cow_angle = angle-90;
	}
	if(!cow_reached || cow_direction != direction){
		do_attach = false;
	}
	if(do_attach){
		node[1]->detach();
		node[0]->add_child(node[1]);
		mat.get_Imat(matrix);
		node[1]->set_world_transf(matrix);
		attach = true;
		do_attach = false;
		cow_reached = false;
	}
	if(direction==0){
		x-=train_speed;
		if(x<=-padding){
			angle = (angle + 90)%360;
			direction = 1;
			do_attach = true;
			}
		}
	else if(direction==1){
		z-=train_speed;
		if(z<=-padding){
			angle = (angle + 90)%360;
			direction = 2;
			do_attach = true;
			}
		}
	else if(direction==2){
		x+=train_speed;
		if(x>=padding){
			angle = (angle + 90)%360;
			direction = 3;
			do_attach = true;
			}
		}
	else if(direction==3){
		z+=train_speed;
		if(z>=padding){
			angle = (angle + 90)%360;
			direction = 0;
			do_attach = true;
			}
		}
	if(!attach && !cow_reached){
		if(cow_direction == 0){
			cow_x += cow_speed;
			if(cow_x>=padding){
				cow_reached = true;
				}
			}
		else if(cow_direction == 1){
			cow_z += cow_speed;
			if(cow_z>=padding)
				cow_reached = true;
			}
		else if(cow_direction == 2){
			cow_x-= cow_speed;
			if(cow_x<=-padding){
				cow_reached = true;
				}
			}
		else if(cow_direction == 3){
			cow_z-= cow_speed;
			if(cow_z<=-padding){
				cow_reached = true;
				}
			}
		mat.get_Tmat(cow_x,-0.53,cow_z,matrix);
		node[1]->set_world_transf(matrix);
		mat.get_Rmat(0,1,0,cow_angle,matrix);
		node[1]->update_world_transf(matrix);
		}
	mat.get_Tmat(x,0,z,matrix);
	node[0]->set_world_transf(matrix);
	mat.get_Rmat(0,1,0,angle,matrix);
	node[0]->update_world_transf(matrix);
	glutPostRedisplay();
}

/* Draw Reference axis */
int View::draw_axis(){
	log_D("Drawing axis");
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(-1.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);

		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,-1.0,0.0);
		glVertex3f(0.0,1.0,0.0);

		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();
	return 0;
	}

/* Refresh view */
int View::refresh(GLfloat *track_matrix,bool *light_status,int cam_loc,bool enable_animation,float train_speed,float cow_speed){
	this->track_matrix = track_matrix; //Rotation by Trackball
	for(int i =0 ;i<6;i++){
		this->light_status[i] = light_status[i];
		}
	this->cam_loc = cam_loc;
	this->enable_animation = enable_animation;
	this->train_speed = train_speed;
	this->cow_speed = cow_speed;
	glutPostRedisplay();
	return 0;
	}

/* Mouse Handler registration */
int View::reg_mouse_handler(void (*mouse_handler)(int button,int state,int x,int y)){
	if(mouse_handler==NULL){
		return 1;
		}
	glutMouseFunc(mouse_handler);
	return 0;
	}

/* Motion Handler registration */
int View::reg_motion_handler(void (*motion_handler)(int x,int y)){
	if(motion_handler==NULL){
		return 1;
		}
	glutMotionFunc(motion_handler);
	return 0;
	}

/* Keyboard Handler registration */
int View::reg_keyboard_handler(void (*keyboard_handler)(unsigned char key,int x,int y)){
	if(keyboard_handler==NULL){
		return 1;
		}
	glutKeyboardFunc(keyboard_handler);
	return 0;
	}

/* Keyboard Special Handler registration */
int View::reg_keyboard_spec_handler(void (*keyboard_spec_handler)(int key,int x,int y)){
	if(keyboard_spec_handler==NULL){
		return 1;
		}
	glutSpecialFunc(keyboard_spec_handler);
	return 0;
	}

int View::set_camera()
{
	Quaternion q;
	Vector curr_pos, curr_up, curr_lookat;
	Vector spot_pos,spot_direction;
	spot_pos.x = spot_pos.y = spot_pos.z = 0.0f;
	spot_pos.y = -0.5f;
	Matrix mat_obj;
	float *mat = new float[16];
	if(cam_loc == ON_OBJ_A){
		animation->get_camera(curr_pos,curr_lookat,curr_up,ON_OBJ_A);
		mat_obj.get_Imat(mat);
		node[0]->get_global_world_tansf(mat);
		mat_obj.get_transf_vector(mat,curr_pos);
		mat_obj.get_transf_vector(mat,spot_pos);
		camera->config_camera(curr_pos,curr_lookat,curr_up);
		}
	else if(cam_loc==ON_OBJ_B){
		/* Get configuration of camera on object B */
		animation->get_camera(curr_pos,curr_lookat,curr_up,ON_OBJ_B);
		mat_obj.get_Imat(mat);
		node[1]->get_global_world_tansf(mat);
		mat_obj.get_transf_vector(mat,curr_pos);
		camera->config_camera(curr_pos,curr_lookat,curr_up);
		}
	else if(cam_loc == ON_OBJ_C){
		/* Get configuration of camera on object C */
		animation->get_camera(curr_pos,curr_lookat,curr_up,ON_OBJ_C);
		mat_obj.get_Imat(mat);
		node[2]->get_global_world_tansf(mat);
		mat_obj.get_transf_vector(mat,curr_pos);
		camera->config_camera(curr_pos,curr_lookat,curr_up);
		}
	else{
		/* Camera in space */
		if(track_matrix){
			q.x = track_matrix[0];
			q.y = track_matrix[1];
			q.z = track_matrix[2];
			q.w = track_matrix[3];
			}
		~q;
		camera->rotate_camera(q);
		}

	/* Set spotlight moving along train */
	mat_obj.get_Imat(mat);
	node[0]->get_global_world_tansf(mat);
	mat_obj.get_transf_vector(mat,spot_pos);
	spot_direction.x = spot_direction.z = 0.0f;
	spot_direction.y = -0.5f;
	animation->set_spot_light(spot_pos,spot_direction);
	delete mat;
	camera->set_camera();
	return 0;
}

/*
 * Set position for fixed light
 * To make light fix call it after camera setup
 */
int View::set_fixed_light()
{
	GLfloat light_position0[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat light_position1[] = { 0.0, 0.0, -3.0, 1.0 };
	GLfloat light_position2[] = { -3.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position2);

	GLfloat direction[] = {0,2.0,0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	Vector pos,dir;
	animation->get_spot_light(pos,dir);
	set_spotlight(pos,dir);

	/* check which light to enable and which to disable */
	if(light_status[0])
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	if(light_status[1])
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
	if(light_status[2])
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);

	if(light_status[5])
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	return 0;
}

/*
 * Setup moving light(headlight)
 */
int View::set_headlight()
{
	GLfloat qaLightPosition[] = {0.0, 0.0, 3.0, 1};
	GLfloat dirVector0[] = { 0.3, -0.50, -4.0, 0.0};

	glLightfv(GL_LIGHT3, GL_POSITION, qaLightPosition);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 5.0);// set cutoff angle
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dirVector0);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 1); // set focusing strength
    if(light_status[3])
		glEnable(GL_LIGHT3);
	else
		glDisable(GL_LIGHT3);
	return 0;
}

/* Set spotlight on moving train */
int View::set_spotlight(Vector& pos,Vector &direction)
{
	GLfloat qaLightPosition[] = {pos.x, pos.y,pos.z, 1};
	GLfloat dirVector0[] = { direction.x, direction.y,direction.z, 0.0};

	glLightfv(GL_LIGHT4, GL_POSITION, qaLightPosition);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 10.0);// set cutoff angle
    glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dirVector0);
    glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 1.0); // set focusing strength
    if(light_status[4]){
		glEnable(GL_LIGHT4);
		}
	else{
		glDisable(GL_LIGHT4);
		}
	return 0;
}

/*
 * Chage texture of selected object
 */
int View::update_tex(string &tex_path,int obj)
{
	/*
	 * Choose object and update texture
	 */
	SceneNode * node = NULL;
	Model * mdl = NULL;
	if(obj==1){
		cube->update_tex(tex_path);
	}
	else{
		node = scene->get_scenenode(obj);
		if(!node){
			return 1;
			}
		mdl = node->get_model();
		if(!mdl){
			return 2;
			}
		mdl->update_tex(tex_path);
		}
	glutPostRedisplay();
	return 0;
}

/*
 * Change texture mode of selected model(OpenGL/Manual)
 */
int View::update_tex_mode(int mode,int obj)
{
	SceneNode * node = NULL;
	Model * mdl = NULL;
	node = scene->get_scenenode(obj);
	if(!node){
		return 1;
	}
	mdl = node->get_model();
	if(!mdl){
		return 2;
		}
	mdl->update_tex_mode(mode);
	glutPostRedisplay();
	return 0;
}

/* sets flag do_detach */
int View::detach_node()
{
	do_detach = true;
	return 0;
}