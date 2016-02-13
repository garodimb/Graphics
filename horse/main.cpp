
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "quaternion.h"
#include "model.h"
#include "cube.h"
#include "trackball.h"

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

/* Scaling Factors */
#define ZOOMIN_FACTOR_X 1.2
#define ZOOMIN_FACTOR_Y 1.2
#define ZOOMOUT_FACTOR_X 1/1.2
#define ZOOMOUT_FACTOR_Y 1/1.2

/* Rotation Angle */
#define ROTATE_ANGLE 5.0
#define FL_DRAG 1
#define FL_PUSH 0

/* Initial Translation for visibility of Ref axes */
#define TRANS_X 20.0
#define TRANS_Y 20.0

#define TRANS_MODE_H 1
#define TRANS_MODE_V 2
#define TRANS_MODE_D 3

using namespace std;

/* Window init */
int initWindow(char *title,int w,int h);
void init(void);
void reshape(int w,int h);
void display(void);

/* Callbacks */
void mouse_handler(int button,int state,int x,int y);
void motion_handler(int x,int y);
void keyboard_handler(unsigned char key,int x,int y);
void keyboard_spec_handler(int key,int x,int y);
void trackball_handler(int event,int xx,int yy);

/* Transformations */
int trans_by_user(GLint x,GLint y);
int rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);
int reset();

/* UI Related */
int draw_axis();

/* Global Variables */
Point trans_point;

int screen_width,screen_height;
int TRANS_MODE=TRANS_MODE_D;


/*	3D Modelling */
Cube cube(2.0f);
Trackball trackball ;
GLint x_ang = 0;
GLint y_ang = 0;

GLint dist = 5.0;

/* Transformations */
GLfloat rotate_x 	= 0.0; // X Rotation
GLfloat rotate_y 	= 0.0; // Y Rotation
GLfloat z_distance 	= 5.0f; // Camera distance
GLfloat scale_all	= 1.0f; // Uniform scaling in all direction
GLfloat trans_x  	= 0.0f; // X translation
GLfloat trans_y  	= 0.0f; // Y translation
GLfloat trans_z  	= 0.0f; // Z translation


/* Initialize windowing system */
int initWindow(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	screen_width=w;
	screen_height=h;
	glutInitWindowPosition(100,100);
	glutCreateWindow(title);
	return 0;
	}
	
/* Init canvas */	
void init(void){
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat light_diffuse[] = { 0.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_NORMALIZE); //Dont care about performance now
	glShadeModel (GL_SMOOTH);
	
	/* Disable LIGHTING to view only colored cube with different colors*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	}

/* Mouse Handler */
void mouse_handler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON){
	//	trackball_handler(FL_PUSH,x,y);
	}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		log_D("Right button Pressed at ("<< x <<","<< y <<")");
			/* Pick and translate */
			trans_by_user(x-TRANS_X,screen_height-y-TRANS_Y);
		}
	}

/* Mouse motion Handler */
void motion_handler(int x, int y){
	trackball_handler(FL_DRAG,x,y);
	}

/* Keyboard Handler */	
void keyboard_handler(unsigned char key,int x,int y){
	log_D("Key Pressed: " << (int)key);
	if(key==KEY_ESC){
		log_I("ESC key pressed. Exiting Program");
		exit(0);
		}
	else if(key==KEY_ENTER){
		log_I("Resetting MODELVIEW");
		reset();
		glutPostRedisplay();
		}
	else if(key==ZOOMIN){
		log_I("Moving Camera Near");
		z_distance-=0.1;
		glutPostRedisplay();
		}
	else if(key==ZOOMOUT){
		log_I("Moving Camera far");
		z_distance+=0.1;
		glutPostRedisplay();
		}
	else if(key == KEY_X){
		log_I("Translating +X");
		trans_x += 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_x){
		log_I("Translating -X");
		trans_x -= 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_Y){
		log_I("Translating +Y");
		trans_y += 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_y){
		log_I("Translating -Y");
		trans_y -= 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_Z){
		log_I("Translating +Z");
		trans_z += 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_z){
		log_I("Translating -Z");
		trans_z -= 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_S){
		log_I("Scaling +X, +Y, +Z");
		scale_all += 0.1f;
		glutPostRedisplay();
		}
	else if(key == KEY_s){
		log_I("Scaling -X, -Y, -Z");
		scale_all -= 0.1f;
		glutPostRedisplay();
		}
	}
	
/* Keyboard Special keys handler */	
void keyboard_spec_handler(int key,int x,int y){
	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT){
		rotate_y += 5;
		}
	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT){
		rotate_y -= 5;
	}
	else if (key == GLUT_KEY_UP){
		rotate_x += 5;
	}
	else if (key == GLUT_KEY_DOWN){
		rotate_x -= 5;
	}
	//  Request display update
	glutPostRedisplay();
	}

/* Trackball Handler */
void trackball_handler(int event,int xx,int yy){
  float matrix[4][4];
	if ( event == FL_PUSH ) {
      x_ang = xx ;
      y_ang = yy ;
      trackball.rotate( 0,0,0,0 ) ;
    }
    else if (( event == FL_DRAG ) ) {
		trackball.rotate((2.0 * x_ang - screen_width) / float(screen_width),
             (screen_height - 2.0 * y_ang) / float(screen_height),
             (2.0 * xx - screen_width) / float(screen_width),
             (screen_height - 2.0 * yy) / float(screen_height));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    trackball.rotationMatrix(matrix);
    glMultMatrixf((float *)matrix);
    x_ang = xx ; y_ang = yy ;
    glutPostRedisplay() ;
    }
}

/* Do Translation by user defined distance */
int trans_by_user(GLint x,GLint y){
	GLfloat x_dist,y_dist,dist;
	static GLint t_point_count = -1;
	if(t_point_count==-1){
		/* Add first point for finding translation distance */
		trans_point.x = x;
		trans_point.y = y;
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
		glTranslatef(x_dist,y_dist,0.0);
		glutPostRedisplay();
		}
	return 0;
	}

/* Rotation using Quaternion*/
int rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
	GLfloat pmatrix[16];
	Quaternion quaternion;
	quaternion.CreateFromAxisAngle(x,y,z,angle);
	quaternion.CreateMatrix(pmatrix);
	glMultMatrixf(pmatrix);
	return 0;
	}

/* Reset all transformations */
int reset(){
	x_ang = 0;
	y_ang = 0;

	dist = 5.0;

	rotate_x 	= 0.0; // X Rotation
	rotate_y 	= 0.0; // Y Rotation
	z_distance 	= 5.0f; // Camera distance
	scale_all	= 1.0f; // Uniform scaling in all direction
	trans_x		= 0.0f; // X translation
	trans_y		= 0.0f; // Y translation
	trans_z		= 0.0f; // Z translation
	}

/* Reshape Handler */
void reshape(int w,int h){
	log_D("Handling reshape");
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, w/h, 0.1f,15.0f);
	log_D("Window Width: "<<w<<", Height: "<<h);
	screen_height=h;
	screen_width=w;
	}
	
/* Display Handler */
void display(){
	log_D("Displaying content");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/* Move camera using z so to get fill of zooming */
	/* Eye, Center and Up vector */	
	gluLookAt(0.0, 0.0,z_distance, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
	glTranslatef(trans_x,trans_y,trans_z);
	glScalef(scale_all,scale_all,scale_all);
	rotate(1.0,0.0,0.0,rotate_x);
	rotate(0.0,1.0,0.0,rotate_y);
	cube.display();
	draw_axis();
	Model::display();
	glutSwapBuffers();
	}

/* Draw Reference axis */	
int draw_axis(){
	log_D("Drawing axis");
	glBegin(GL_LINES);
		glColor3f(1.0,1.0,1.0);
		glVertex3f(-1.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		glVertex3f(0.0,-1.0,0.0);
		glVertex3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();
	glPopMatrix();
	return 0;
	}
	
int main(int argc,char **argv){
	glutInit(&argc,argv);
	initWindow("3D Modelling",1000,500);
	init();
	Model::init((char *)"bunny.ply");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse_handler);
	//glutMotionFunc(motion_handler);
	glutKeyboardFunc(keyboard_handler);
	glutSpecialFunc(keyboard_spec_handler);
	glutMainLoop();
	return 0;
	}
