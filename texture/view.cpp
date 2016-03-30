#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <view.h>

View *view;

static void on_display(){
	view->display();
	}

static void on_reshape(int w,int h){
	view->reshape(w,h);
	}

View::View(int argc,char **argv){
	track_matrix= NULL; // Trackball Matrix
	rotate_x 	= 0.0f; // X Rotation
	rotate_y 	= 0.0f; // Y Rotation
	z_distance 	= 5.0f; // Camera distance
	scale_all	= 1.0f; // Uniform scaling in all direction
	trans_x  	= 0.0f; // X translation
	trans_y  	= 0.0f; // Y translation
	trans_z  	= 0.0f; // Z translation
	glutInit(&argc,argv);
	init_window("3D Modelling",1000,500);
	init();
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	view = this;
	cube = new Cube(2.0f);
	model = new Model();
	if(argc==2){
		model->read_ply(argv[1]);
		}
	else{
		model->read_ply((char *)"plyfiles/Armadillo.ply");
		}
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

	#ifndef DEF_LIGHT
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_diffuse[] = {0.8,0.8,0.8};
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 1.0, 0.0 };

	/* Disable LIGHTING to view only colored cube with different colors*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	#else
	GLfloat qaAmbientLight[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat qaDiffuseLight[] = {1, 0, 1, 1.0};
	GLfloat qaSpecularLight[]  = {1.0, 1.0, 1.0, 1.0};
	GLfloat emitLight[] = {0.9, 0.9, 0.9, 0.01};
	GLfloat Noemit[] = {0.0, 0.0, 0.0, 1.0};
    // Light source position
	GLfloat qaLightPosition[] = {0, 0.0, 1.0, 1};
	GLfloat qaLightDirection[] = {1, 1, 1, 0};
	GLfloat dirVector0[] = { 0.0, 0.0, -1.0, 0.0};

	//glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 2.0);// set cutoff angle
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dirVector0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1); // set focusing strength
	#endif
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	return 0;
	}

int View::init(void){

	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_NORMALIZE); //Dont care about performance now
	glShadeModel (GL_SMOOTH);
	init_lighting();
	glEnable(GL_DEPTH_TEST);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/* Move camera using z so to get fill of zooming */
	/* Eye, Center and Up vector */
	gluLookAt(0.0, 0.0,-z_distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(trans_x,trans_y,trans_z);
	glScalef(scale_all,scale_all,scale_all);
	log_D("Rotate X: "<<rotate_x<<", Rotate Y:"<<rotate_y);
	rotate(1.0,0.0,0.0,rotate_x);
	rotate(0.0,1.0,0.0,rotate_y);
	if(track_matrix)
		glMultMatrixf(track_matrix);
	glDisable(GL_LIGHTING);
	cube->display();
	draw_axis();
	glEnable(GL_LIGHTING);
	model->display();
	glutSwapBuffers();
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

/* Rotate using Quternion */
int View::rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
	GLfloat pmatrix[16];
	Quaternion quaternion;
	quaternion.CreateFromAxisAngle(x,y,z,angle);
	quaternion.CreateMatrix(pmatrix);
	glMultMatrixf(pmatrix);
	return 0;
	}

/* Refresh view */
int View::refresh(GLfloat rotate_x,GLfloat rotate_y,GLfloat z_distance,GLfloat scale_all,GLfloat trans_x,GLfloat trans_y,GLfloat trans_z,GLfloat *track_matrix){
			this->rotate_x 	= rotate_x; // X Rotation
			this->rotate_y 	= rotate_y; // Y Rotation
			this->z_distance= z_distance; // Camera distance
			this->scale_all	= scale_all; // Uniform scaling in all direction
			this->trans_x  	= trans_x; // X translation
			this->trans_y  	= trans_y; // Y translation
			this->trans_z  	= trans_z; // Z translation
			this->track_matrix = track_matrix; //Rotation by Trackball
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


